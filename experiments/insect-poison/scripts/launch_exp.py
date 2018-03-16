import socket
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
  netcat("loadmodule opencog/attention/libattention.so")

def load_experiment_module() :
  netcat("loadmodule opencog/experiments/insect-poison/libinsect-poison-exp.so")

def start_ecan() :
  nectcat("start-ecan")

def start_logger() :
  nectcat("start-logger")

def topic_switched() :
  nectcat("topic-switched 1")

def dump_af_stat(zfile) :
  nectcat("dump-af-stat "+zfile)

def parse_sent_file(zfile) :
  nectcat('(parse-all nlp-parse "'+zfile+'")')

def parse_sent(sent) :
  nectcat('(nlp-parse "'+sent+'")')


def start_word_stimulation(stimulus) :
  nectcat('(nlp-start-stimulation '+str(stimulus)+')')


load_files = ["/home/misgana/Desktop/ECAN/db/conceptnet4.scm",
    "/home/misgana/Desktop/ECAN/db/wordnet.scm",
    "/home/misgana/Desktop/ECAN/db/adagram_sm_links.scm"]

if __name__ == "__main__" :

  scm_load(load_files)
