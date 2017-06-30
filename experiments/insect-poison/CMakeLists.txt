INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

ADD_LIBRARY(insect-poison-exp SHARED
    InsectPoisonExpModule
    LoggerAgent
)

TARGET_LINK_LIBRARIES(insect-poison-exp
	${ATOMSPACE_LIBRARIES}
	attention
	server
)


ADD_DEPENDENCIES(insect-poison-exp attention)
ADD_DEPENDENCIES(insect-poison-exp server)
ADD_DEPENDENCIES(insect-poison-exp cogserver)
