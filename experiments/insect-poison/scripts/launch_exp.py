#!/usr/bin/python
import datetime
import numpy as np
import matplotlib.pyplot as plt
import os
import socket
import sys
import time


# This implements netcat in python.
def netcat(content, hostname = "localhost", port = 17001) :
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
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
  s.close()
  return 0  # zero means success

def scm_load(files) :
  for f in files:
    start_time = time.time()
    tmp = f.split('/')

    print "Loading %s [%s]" % (tmp[len(tmp)-1], f)
    netcat("(primitive-load \"" + f + "\")")
    print "Finished loading in %d sec" % (time.time() - start_time)

def load_ecan_module() :
  print "Loading libattention module"
  netcat("loadmodule opencog/attention/libattention.so")

def load_experiment_module() :
  print "Loading libinsect-poison-exp module"
  netcat("loadmodule experiments/insect-poison/libinsect-poison-exp.so")

def start_server(exec_path, exec_name) :
  DEVNULL = open(os.devnull, 'wb')
  pid = subprocess.Popen(exec_path, stdin=DEVNULL, stdout=DEVNULL,
                         stderr=DEVNULL, close_fds=True).pid
  print "     started %s \n" % (exec_name)
  return pid

def restart_server(pid, exec_path, exec_name) :
  print "     restarting %s \n" % (exec_name)
  kill_process(pid)
  pid = start_server(exec_path, exec_name)
  print "     restarting completed."
  return pid
    
def kill_process(pid) :
  os.kill(int(pid), signal.SIGKILL)


ecan_started = False
def start_ecan(multithreaded_mode) :
  global ecan_started
  if not ecan_started :
      print "Starting ECAN agents in "
      if  multithreaded_mode :
         print " multithreaded mode.\n"
         netcat("start-ecan")
      else:
          print " single threaed mode.\n"
          netcat('agents-start opencog::AFImportanceDiffusionAgent')
          netcat('agents-start opencog::WAImportanceDiffusionAgent')
          netcat('agents-start opencog::AFRenctCollectionAgent')
          netcat('agents-start opencog::WARentCollectionAgent')
          netcat('agents-start opencog::HebbianUpdatingAgent')
          netcat('agents-start opencog::HebbianCreationAgent')
      
      # Disable Hebbian agents
      #netcat('agents-stop opencog::HebbianCreationAgent')
      #netcat('agents-stop opencog::HebbianUpdatingAgent')
      ecan_started = True


logger_started = False
def start_logger() :
  global logger_started
  if not logger_started:
     print "Starting experiment logger agent."
     netcat("start-logger")
     logger_Started = True

def topic_switched(is_on) :
  netcat("topic-switched "+(is_on==True and '1' or '0'))

def dump_percentage_af(zfile) :
  netcat("dump-af-size-log "+zfile)

def dump_af_stat(zfile) :
  netcat("dump-af-stat "+zfile)

def parse_sent_file(zfile) :
  netcat('(parse-all nlp-parse "'+zfile+'")')

def parse_sent(sent) :
  netcat('(nlp-parse "'+sent+'")')


def start_word_stimulation(stimulus) :
  netcat('(nlp-start-stimulation '+str(stimulus)+')')

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

def save(string, fpath):
  f=open(fpath,'w')
  f.write(string)
  f.close()
 
def experiment_1(): 
  start_logger()
  start_ecan()
 
  start_word_stimulation(400)
  
  print "Parsing insect sentences."
  parse_sent_file(SENT_DIR+"/insects-100.sent")
  print "Dumping log data."
  
  dump_af_stat("pydump-after-insect")

  topic_switched(True)
  
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
  save(str(non_nlp_words), DATA_DIR+"/log/"+datetime.datetime.now().strftime("%Y-%m-%d-%H:%M:%S")+".log")

def experiment_2():
  def switch_back_to_insect():
    start_logger()
    start_ecan()
    start_word_stimulation(250)

    print "Parsing insect sentences."
    parse_sent_file(SENT_DIR+"/insects-100.sent")
    print "Dumping log data."
    time.sleep(10)
    dump_af_stat("pydump-after-insect")

    topic_switched(True)

    print "Parsing poison sentences."
    parse_sent_file(SENT_DIR+"/poisons-50.sent")

    print "Parsing insect sentences once again."
    parse_sent_file(SENT_DIR+"/insect-50.sent")

    print "Dumping log data."
    dump_af_stat("pydump-percentage-af")

  def switch_to_cars():
    start_logger()
    start_ecan()
    start_word_stimulation(250)

    print "Parsing insect sentences."
    parse_sent_file(SENT_DIR+"/insects-100.sent")
    print "Dumping log data."
    dump_af_stat("pydump-exp2_1")

    topic_switched(True)

    print "Parsing poison sentences."
    parse_sent_file(SENT_DIR+"/poisons-50.sent")

    print "Parsing car sentences."
    parse_sent_file(SENT_DIR+"/cars-50.sent")
    
    print "Dumping log data."
    dump_af_stat("pydump-exp2_2")

  print "Experiment 2 running....................."
  print "   Running case 1"
  switch_back_to_insect()
  print "   Finished case 1"

  print "   Restarting the cogserver." 
  # Restart cogserver
  global pid_cog
  global pid_relex
  pid_cog = restart_server(pid_cog, COGSERVER, 'cogserver')
  pid_relex = restart_server(pid_relex, RELEX, 'relex')
  scm_load(LOAD_FILES)
  load_experiment_module()
  load_ecan_module()
  
  print "   Running case 2"
  switch_to_cars()
  print "   Done case 2"

def experiment_3():
  # All about HebbianLinks how they could estabilish weak links which stabilize
  # the dynamics.
  #start_logger()
  start_ecan()

  start_word_stimulation(250)

  print "Parsing the SEW(simple english wikipedia)."
  #Start parsing the SEW
  script_dir=BASE_DIR+'/opencog/nlp/learn/run'
  os.system(script_dir+'/wiki-ss-en.sh'+' '+script_dir)

  #Rerun experiment 1
  experiment_1()

def plot_save(af_stat_file, plot_path):
    #at_time(sec), nonNLP_percentage, insect_percentage, poison_percentage, insecticide_percentage
    time = []
    non_nlp = []
    insect = []
    poison = []
    insecticide = []
    line_no = 0
    #Expected CSV format
    with open(af_stat_file) as f:
      for line in f:
        if line_no == 0:
          line_no = line_no + 1
          continue
        line = [x.strip() for x in line.split(',') ]
        time.append(float(line[0]))
        non_nlp.append(float(line[1]))
        insect.append(float(line[2]))
        poison.append(float(line[3]))
        insecticide.append(float(line[4]))
    
    plt.plot(time, insect)
    plt.plot(time, poison)
    plt.plot(time, insecticide)
    plt.plot(time, non_nlp)
    plt.legend(['Insect', 'Poison', 'Insecticide', 'Non-nlp'], loc='best') 
    plt.ylabel('Percentage in AF')
    #plt.show('Time in AF(sec)')
    plt.savefig(plot_path+"/plot.png")

def sanity_check():
  pass

BASE_DIR = sys.argv[2]
DATA_DIR = BASE_DIR+"/experiments/insect-poison/data"
SENT_DIR = DATA_DIR+"/sentences"
WORD_DIR = DATA_DIR+"/words"
LOAD_FILES = [DATA_DIR+"/kb/conceptnet4.scm",
              DATA_DIR+"/kb/wordnet.scm",
              DATA_DIR+"/kb/adagram_sm_links.scm"]

COGSERVER = BASE_DIR+"opencog/build/opencog/cogserver/server/cogserver"
RELEX     = BASE_DIR+"relex/opencog-server.sh"

pid_cog
pid_relex

if __name__ == "__main__" :
  experiments = {1:experiment_1, 2:experiment_2, 3:experiment_3}
  conf_str = []
  #TODO Read and load load conf file.
  #TODO Sanity check.
  expid = sys.argv[1]
  if(not(expid and expid in ["1","2","3"])):
    print "Please specify a valid experiment id \n"
    print "   launch_exp.py <1|2|3>"
    sys.exit(0)
  # start cogserver
  global pid_cog
  global pid_relex
  pid_cog = start_server(COGSERVER, 'cogserver')
  # start relex  
  pid_relex = start_server(RELEX, 'relex')
  
  experiment = experiments[int(expid)]
  for i in range(0, len(conf_str)) :
     print "Experiment %d started. \n" %(i+1)
     # Load KB and modules. It is necessary to load 
     # the knowledge base before starting logging agent.
     scm_load(LOAD_FILES)
     load_experiment_module()
     load_ecan_module()
     netcat(conf) #load ecan conf param
     expeirment()
     print "Dumping af stat pecentage\n"
     time.sleep(5)
     dump_percentage_af("pydump-percentage")
     #TODO create a sensible dir name.
     dir_name = ""
     path = DATA_DIR+"/log/"+dir_name
     os.system("mkdir "+path)
     os.system("cp "+BUILD_DIR+"/*.data  "+path)
     plot_save(path+"/pydump-percentage-af.data", path)
     # restart cogserver and relex
     pid_cog = restart_server(pid_cog, COGSERVER, 'cogserver')
     pid_relex = restart_server(pid_relex, RELEX, 'relex')
