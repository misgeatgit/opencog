
##Thoughts on the opencog rule engine algorithm and architectural design

The following paragraphs try to address use cases and architectural goals for the opencog rule engine (OCRE)

###Architectural overview
The Rule engine will have two major high level components

####A PLN Module 
 The purpose of this is similar to  event/condition/action rule engines. I thought this is the perfect and suitable way of applying generic PLN rules by letting them do inferences on some cycles.
 The performance could be increased by storing indexes of previously made inference.

#### An API
  The purpose of this is to be used as Production inference rule engine. I envision to have a c++ API to be called from other C++ modules and a scheme binding.This is suitable for specialized knowledge inferece
  say playing some rule based game. Another mind agent who is playing the game could consult the API inference related stuff .
  
  the parameters

**restrictions** what set of values should OCRE look for the variable's grounding (increase performance and guides search)
**a config**
	
	*containing rules to apply
	*mutexes mutual exclusions	

**query**  A variable containing query ( the non variable containing query will be loaded with opencog with a config file containing rules and mutexes in it)
  

