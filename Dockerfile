FROM centos:7

RUN yum -y install \
  gcc \
  valgrind \
  gdb \
  gprof \
  make \
  doxygen \
  vim-enhanced \
  git

