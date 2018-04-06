using AdaGram

# /home/misgana/Desktop/adagram-wiki-model1
PATH_TO_MODEL = ARGS[1];
SCM = ""

insecticides=[]
insects = []
poisons = []
function populate_words ()
    BASE_DIR = "/home/misgana/OPENCOG/opencog/experiments/insect-poison/data/words"
    open(string (BASE_DIR,"/insecticide.words")) do f
       for w in eachline (f)
               push! (insecticides, w)
       end
    end
    
    open(string (BASE_DIR,"/insects.words")) do f
            for w in eachline (f)

                    push! (insects, strip (w, '\n'))
            end
    end
    
    open(string (BASE_DIR,"/poison.words")) do f
            for w in eachline (f)
                    push! (poisons, strip (w, '\n'))
            end
    end
end

# Creates similariy link between words w1 and w2.
function create_as_similarity_link(w1, w2)
        #println(string ( w1,",",w2))
        w1 = strip (w1, '\n')
        w2 = strip (w2, '\n')
        link = ""
        try
                strength = similarity(vm, dict, w1, 1, w2, 1)
                strength = round (strength, 3)
                if (strength < 0 || isnan (strength) )
                        strength = 0.0
                end
                link = string("(SimilarityLink (WordNode \"",w1, "\") (WordNode \"",w2,"\") (cog-new-stv ",1.0," 1.0))")
        catch LoadError
                link = string("(SimilarityLink (WordNode \"",w1, "\") (WordNode \"",w2,"\") (cog-new-stv ",0," 1.0))")
                #println("LoadError")
        finally
                return link      
        end
end  

populate_words()
vm, dict = load_model(PATH_TO_MODEL);
# Create insecticide-insect similarity links
for i= 1:length(insecticides)
        global SCM
        for j = 1:length(insects)
                #println(string (insecticides[i],",",insects[j]))
                smlink = create_as_similarity_link(insecticides[i], insects[j])
                SCM *= string(smlink,"\n")
        end
end

# Create insecticide-poison similarity links
for i= 1:length(insecticides)
        global SCM
        for j = 1:length(poisons)
                #println(string (insecticides[i],",",insects[j]))
                smlink = create_as_similarity_link(insecticides[i], poisons[j])
                SCM *= string(smlink,'\n')
        end
end

#print the scheme string to STDOUT
println(SCM) 



