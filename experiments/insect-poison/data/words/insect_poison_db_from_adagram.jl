using AdaGram

# /home/misgana/Desktop/adagram-wiki-model1
PATH_TO_MODEL = ARGS[1];
INSECT_POISON_FILE = ARGS[2];
SCM = ""

word_list=[]
#populate word_list by picking up words from insect poison sentence database
open(INSECT_POISON_FILE) do f
        for wordz in eachline(f)
                # replace undesirable chars
                wordz = replace(wordz,r"[-,--,_]"," ")
                wordz = replace(wordz,r"[\",.,\n,,,;,:]","")                
                global word_list
                for w in split(wordz,' ')
                        w = lowercase(w)
                        exclude = ["he","she","they","them","a","on","of",
                                   "there","why","when","which","whose","where",
                                   "themselves","ourselves","themself","himself",
                                   "in","myself","herself","himself","the","or",
                                   ]
                        if !(w in exclude) && w !="" && w != " "
                                push!(word_list, w)
                        end
                end
        end
end

# Creates similariy link between words w1 and w2.
function create_as_similarity_link(w1, w2)
        link = ""
        try
                strength = similarity(vm, dict, w1, 1, w2, 1)
                strength = round (strength, 3)
                if (strength < 0 || isnan (strength) )
                        strength = 0.0
                end
                link = string("(SimilarityLink (ConceptNode \"",w1, "\") (ConceptNode \"",w2,"\") (cog-new-stv ",strength," 1.0))")
        catch LoadError
                #println("LoadError")
        finally
                return link        
        end
end  

vm, dict = load_model(PATH_TO_MODEL);
for i= 1:length(word_list)
        for j = i+1:length(word_list)
                #println(string (word_list[i],",",word_list[j]))
                smlink = create_as_similarity_link(word_list[i], word_list[j])
                global SCM
                SCM *= string(smlink,"\n")
        end
end

#print the scheme string to STDOUT
println(SCM) 



