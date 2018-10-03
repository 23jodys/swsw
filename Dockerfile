FROM debian:stretch

RUN apt update && apt install -y wget gnupg && rm -rf /var/lib/apt/lists/*
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
RUN echo "deb http://apt.llvm.org/stretch/ llvm-toolchain-stretch-7 main" > /etc/apt/sources.list.d/llvm.list
RUN apt update && apt install -y \
	clang-7 \
	cmocka-doc \
	doxygen \
	git \
	lldb-7 \
	libcmocka-dev \
	libcmocka0 \
	make \
	valgrind \
	vim \
	&& rm -rf /var/lib/apt/lists/*
RUN git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf && ~/.fzf/install
