#!/usr/bin/python
import commands
import datetime
import numpy as np
import matplotlib as mpl
mpl.use('Agg') # For running it without xwindows system
import matplotlib.pyplot as plt
import os
import socket
import subprocess
import sys
import time

OC_DIR = sys.argv[2]
BASE_DIR = OC_DIR+"/opencog"
BUILD_DIR =BASE_DIR+"/build"
DATA_DIR = BASE_DIR+"/experiments/insect-poison/data"
SCRIPT_DIR = BASE_DIR+"/experiments/insect-poison/scripts"
SENT_DIR = DATA_DIR+"/sentences"
WORD_DIR = DATA_DIR+"/words"
LOAD_FILES = [DATA_DIR+"/kb/conceptnet4.scm",
              DATA_DIR+"/kb/wordnet.scm",
              DATA_DIR+"/kb/adagram_sm_links.scm"]

COGSERVER = OC_DIR+"/opencog/build/opencog/cogserver/server/cogserver"
RELEX     = OC_DIR+"/relex/opencog-server.sh"
COGSERVER_PORT = '17001'
RELEX_PORT = '4444'
RUNNING_DIR = os.path.abspath(os.path.dirname(sys.argv[0])) 

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
  netcat("loadmodule "+BUILD_DIR+"/opencog/attention/libattention.so")

def load_experiment_module() :
  print "Loading libinsect-poison-exp module"
  netcat("loadmodule "+BUILD_DIR+"/experiments/insect-poison/libinsect-poison-exp.so")

def start_server(exec_path, exec_name) :
  DEVNULL = open(os.devnull, 'wb')
  subprocess.Popen(exec_path, stdin=DEVNULL, stdout=DEVNULL, stderr=DEVNULL)
  time.sleep(3)
  output = commands.getstatusoutput('lsof -ti :17001')[1]
  if not output:
    print "Unable to start cogserer. Exiting"
    sys.exit()
  print "started %s [pid: %s] \n" % (exec_name, output)

def restart_server(exec_path, exec_name) :
  print "restarting %s \n" % (exec_name)
  kill_process(COGSERVER_PORT)
  start_server(exec_path, exec_name)
  print "restarting completed."
   
def start_relex():
  DEVNULL = open(os.devnull, 'wb')
  cmd = "cd " + OC_DIR+"/relex \n"
  with open(RELEX, 'r') as f:
    for line in f:
        cmd = cmd + line 
  tmpf = open("/tmp/relex.stdout", 'w')
  subprocess.Popen(['/bin/bash', '-c', cmd], stderr=DEVNULL, stdout=tmpf)
  time.sleep(3)
  output = commands.getstatusoutput('lsof -ti :4444')[1]
  if not output:
    print "Unable to start relex. Exiting"
    sys.exit()

  print "started %s [pid: %s] \n" % ('relex', output)

def restart_relex():
  print "restarting %s \n" % ('relex')
  kill_process(RELEX_PORT)
  start_relex()
  print "restarting completed."
  
def kill_process(port) :
  os.system('kill -9 $(lsof -ti :'+port+')')
  time.sleep(2)
  #os.kill(int(pid), signal.SIGKILL)

def load_experiment_resources():
     scm_load(LOAD_FILES)
     load_experiment_module()
     load_ecan_module()

ecan_started = False
def start_ecan(multithreaded_mode=True) :
  global ecan_started
  if not ecan_started :
      print "Starting ECAN agents in "
      if  multithreaded_mode :
          print " multithreaded mode.\n"
          netcat("start-ecan")
          # Disable Hebbian agents
          #netcat('agents-stop opencog::HebbianCreationAgent')
          #netcat('agents-stop opencog::HebbianUpdatingAgent')
      else:
          print " single threaed mode.\n"
          netcat('agents-start opencog::AFImportanceDiffusionAgent')
          netcat('agents-start opencog::WAImportanceDiffusionAgent')
          netcat('agents-start opencog::AFRenctCollectionAgent')
          netcat('agents-start opencog::WARentCollectionAgent')
          netcat('agents-start opencog::HebbianUpdatingAgent')
          netcat('agents-start opencog::HebbianCreationAgent')
      
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
 
  #start_word_stimulation(400)
  
  print "Parsing insect sentences."
  start_time = time.time()
  parse_sent_file(SENT_DIR+"/insects-100.sent")
  print "Took %d sec." % (time.time() - start_time)
  print "Dumping log data."
  
  dump_af_stat("pydump-after-insect")

  topic_switched(True)
  
  print "Parsing poison sentences."
  start_time = time.time()
  parse_sent_file(SENT_DIR+"/poisons-50.sent")
  print "Took %d sec." % (time.time() - start_time)
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
  with open(RUNNING_DIR+"/pydump-after-poison.data", 'r') as logf:
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
  print "Running case 1"
  switch_back_to_insect()
  print "Finished case 1"

  print "Restarting the cogserver." 
  # Restart cogserver
  restart_server(COGSERVER, 'cogserver')
  restart_relex()
  scm_load(LOAD_FILES)
  load_experiment_module()
  load_ecan_module()
  
  print "Running case 2"
  switch_to_cars()
  print "Done case 2"

def experiment_3():
  # All about HebbianLinks how they could estabilish weak links which stabilize
  # the dynamics.
  #start_logger()
  start_ecan()

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
    plt.xlabel('Time(sec)')
    plt.savefig(plot_path+"/plot.png")
    plt.clf()

def sanity_check():
  pass


if __name__ == "__main__" :
  experiments = {1:experiment_1, 2:experiment_2, 3:experiment_3}
  conf_str = []
  # Read and load load conf file.
  with open(SCRIPT_DIR+'/experiment.ini') as conf_file:
    for line in conf_file:
      if line.strip() and line.strip()[0] != ';':
        conf_str.append(line)

  #TODO Sanity check.
  expid = sys.argv[1]
  if(not(expid and expid in ["1","2","3"])):
    print "Please specify a valid experiment id \n"
    print "   launch_exp.py <1|2|3>"
    sys.exit(0)

  # start cogserver
  start_server(COGSERVER, 'cogserver')
  # start relex  
  start_relex()

  netcat('(use-modules (opencog nlp))(use-modules (opencog nlp relex2logic))(use-modules (opencog nlp chatbot))')

  experiment = experiments[int(expid)]
  for i in range(0, len(conf_str)) :
     print "--------------------Experiment %s started.------------------------- \n" %(sys.argv[3])
     print "Settings %s" % (conf_str[i])
     # Load KB and modules. It is necessary to load 
     # the knowledge base before starting logging agent.
     load_experiment_resources()
     # Load Params. XXX MAX_AF_SIZE won't work since it is only
     # set once. And should be done after load_experiment_resources().
     netcat(conf_str[i])
     print "Settings are now in effect."
     experiment()
     time.sleep(10)
     print "Dumping af stat pecentage"
     dump_percentage_af("pydump-percentage")
     time.sleep(1)
     # Create a sensible dir name.
     dir_name = "setting_"+str(sys.argv[3])
     path = DATA_DIR+"/log/"+dir_name
     os.system("mkdir "+path)
     os.system("cp "+RUNNING_DIR+"/*.data  "+path)
     plot_save(path+"/pydump-percentage-af.data", path)
     with open(path+"/settings.txt", 'w') as f:
       f.write(conf_str[i])
     # restart cogserver and relex
     if i == len(conf_str)-1:
         kill_process(COGSERVER_PORT)
         kill_process(RELEX_PORT)
     else:
         restart_server(COGSERVER, 'cogserver')
         restart_relex()
