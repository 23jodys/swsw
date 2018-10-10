cmake_minimum_required (VERSION 3.5)
project (swsw VERSION 1.0.0 LANGUAGES C)
add_executable(test_sw src/test_sw.c src/score_matrix.c src/sw.c src/alignment.c src/sds/sds.c)
target_link_libraries(test_sw cmocka)

add_executable(test_alignment src/test_alignment.c src/alignment.c src/sds/sds.c)
target_link_libraries(test_alignment cmocka)
