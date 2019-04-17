(use-modules (opencog))
(use-modules (opencog octomap))
(use-modules (opencog pointmem))

(define znode_0 (Node "zitem_0"))

; One octomap object to store locations of items
(define zmap (OctomapNode "zmap"))

; Insert items and their location to the octomap
(define zvalue_0 (OctoValue  (ListLink znode_0 zmap)))

(define key (PredicateNode "spacetime") )

; setting values
(cog-set-value! znode_0 key zvalue_0)

; getting values
(cog-value znode_0 key)

; list ref
(define v (cog-value znode_0 key))
(cog-value->list v)
(cog-value->ref  v 1)

