; New pointmem interface usgage example
(use-modules (opencog pointmem))
(use-modules (opencog octomap))

; step one - create the map with the octomapNode as the map's name
(define ocmap (OctomapNode "ocmap"))
;create map ov sets ocmap's internal TimeOctomap Object with a new one
(cog-pointmem-create-map-ov  ocmap  (ListLink (NumberNode "0.5") (NumberNode "1") (NumberNode "1")))

; step two - create octovalues with an item and the octovalue
(define item (ConceptNode "item_0"))

; Supported queries:
;  - change location of the item
(cog-pointmem-map-atom-ov ocmap item (FloatValue 1.34 45 55))

;  - get locs of atom returns FloatVlaue
(cog-pointmem-get-locs-of-atom-ov ocmap item)



