import socket
import sys
import time

# This implements netcat in python.
def netcat(content, hostname = "localhost", port = 17001) :
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

  # If the cogserver is down, the connection will fail.
  try:
    s.connect((hostname, port))
  except socket.error as msg:
    print "Connect failed: ", msg
    s.close()
    return 1  # non-zero means failure

  s.sendall(content)
  s.shutdown(socket.SHUT_WR)
  while True:
    data = s.recv(1024)
    if not data or data == "":
      break
  # print "Received:", repr(data)
  # print "Connection closed."
  s.close()
  return 0  # zero means success

def scm_load(files) :
  for f in files:
    start_time = time.time()
    tmp = f.split('/')

    print "Loading %s" % (tmp[len(tmp)-1])
    netcat("(primitive-load \"" + f + "\")")
    print "Finished loading in %d sec" % (time.time() - start_time)

def load_ecan_module() :
  print "Loading libattention module"
  netcat("loadmodule opencog/attention/libattention.so")

def load_experiment_module() :
  print "Loading libinsect-poison-exp module"
  netcat("loadmodule experiments/insect-poison/libinsect-poison-exp.so")

def start_ecan() :
  netcat("start-ecan")
  netcat('agents-stop opencog::HebbianCreationAgent')
  netcat('agents-stop opencog::HebbianUpdatingAgent')


def start_logger() :
  netcat("start-logger")

def topic_switched(is_on) :
  netcat("topic-switched "+(is_on==True and '1' or '0'))

def dump_af_stat(zfile) :
  netcat("dump-af-stat "+zfile)

def parse_sent_file(zfile) :
  netcat('(parse-all nlp-parse "'+zfile+'")')

def parse_sent(sent) :
  netcat('(nlp-parse "'+sent+'")')


def start_word_stimulation(stimulus) :
  netcat('(nlp-start-stimulation '+str(stimulus)+')')


BASE_DIR = "/home/misgana/OPENCOG/opencog"
DATA_DIR = BASE_DIR+"/experiments/insect-poison/data"

SENT_DIR = DATA_DIR+"/sentences"
WORD_DIR = DATA_DIR+"/words"

load_files = ["/home/misgana/Desktop/ECAN/db/conceptnet4.scm",
              "/home/misgana/Desktop/ECAN/db/wordnet.scm",
              DATA_DIR+"/kb/adagram_sm_links.scm"]


def start_pipeline():
  pass

# Atom(uuid), EnteredAt, LastSeenAt, STI, DurationInAF, IsNLPParseOutput, DirectSTI, GainFromSpreading
def extract_log(column, starting_row, file_name):
  col = []
  with open(file_namei, 'r') as log:
    start = 1
    for line in log:
      if(start != starting_row):
        start = start + 1
      else:
        values = line.split(',')
        col.append(values[column-1])

    return col

def experiment_1(): 
  print "Starting ecan and logger agents."
  start_logger()
  start_ecan()
 
  print "Parsing insect sentences."
  parse_sent_file(SENT_DIR+"/insects-100.sent")
  print "Dumping log data."
  dump_af_stat("pydump-after-insect")

  topic_switched(True)
  
  start_word_stimulation(250)
  print "Parsing poison sentences."
  parse_sent_file(SENT_DIR+"/poisons-50.sent")
  print "Dumping log data."
  dump_af_stat("pydump-after-poison")

  
  insecticides = []

  with open(WORD_DIR+"/insecticide.words") as words:
    for w in words:
      insecticides.append(w)

  seen_in_af = []
  non_nlp_words= []
  line_no = 0
  #with open(BASE_DIR+"/build/pydump-after-insect.data", 'r') as logf:
  with open(BASE_DIR+"/build/pydump-after-poison.data", 'r') as logf:
    for log in logf:
      if line_no < 6:
        line_no = line_no + 1
        continue

      #print log
      word = log.split(',')[0].strip()
      seen_in_af.append(word)
      if log.split(',')[5].strip() == '0':
        non_nlp_words.append(word)

  si = set(insecticides)
  saf = set(seen_in_af)

  intersection = saf.intersection(si);

  print "Found %d insecticide related words" % (len(intersection))
  print intersection
  print "Non nlp atoms"
  print non_nlp_words

def experiment_2():
  # Checking how fast attention switching is when a dissimilar topic is parsed
  # as compared to similar topics like in experiment 1
  pass

def experiment_3():
  # All about HebbianLinks how they could estabilish weak links which stabilize
  # the dynamics.
  pass

if __name__ == "__main__" :
  # This is very critical. Load the auxilary data before launching the logging agent.
  scm_load(load_files)

  load_experiment_module()
  load_ecan_module()

  expid = sys.argv[1]
  if(not(expid and expid in ["1","2","3"])):
    print "Please specify a valid experiment id \n"
    print "   launch_exp.py <1|2|3>"
    sys.exit(0)

  if expid == "1":
    experiment_1()

  if expid == "2":
    experiment_2()

  if expid == "3":
    experiment_3()







