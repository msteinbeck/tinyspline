FROM buildpack-deps:stretch
MAINTAINER Marcel Steinbeck

# Install linux build tools and libraries.
RUN apt-get update &&                          \
    apt-get upgrade -y &&                      \
    apt-get install -y --no-install-recommends \
        automake                               \
        bison                                  \
        clang                                  \
        cmake                                  \
        curl                                   \
        default-jdk                            \
        dub                                    \
        file                                   \
        flex                                   \
        gcc-arm-linux-gnueabihf                \
        git                                    \
        lua5.1 liblua5.1-dev                   \
        lua5.2 liblua5.2-dev                   \
        lua5.3 liblua5.3-dev                   \
        luarocks                               \
        maven                                  \
        mingw-w64                              \
        mono-mcs mono-reference-assemblies-2.0 \
        libtool                                \
        octave liboctave-dev                   \
        php-dev                                \
        pkg-config                             \
        python                                 \
        r-base r-cran-rcpp                     \
        texinfo                                \
        wget                                   \
        zip &&                                 \
    wget -P /usr/local/src/swig \
        http://prdownloads.sourceforge.net/swig/swig-4.0.1.tar.gz && \
    tar -xf /usr/local/src/swig/swig-4.0.1.tar.gz \
        -C /usr/local/src/swig \
        --strip 1 && \
    cd /usr/local/src/swig && \
        ./autogen.sh && \
        ./configure && \
        make && \
        make install

# Install macosx build tools (osxcross) and libraries (macports).
ENV PATH /opt/osxcross/target/bin:${PATH}
RUN export SDK_VERSION=10.13 &&                               \
    export MACOSX_DEPLOYMENT_TARGET=${SDK_VERSION} &&         \
    export UNATTENDED=1 &&                                    \
    cd /opt &&                                                \
    git clone https://github.com/tpoechtrager/osxcross.git && \
    cd osxcross &&                                            \
    wget -P tarballs https://github.com/phracker/MacOSX-SDKs/releases/download/${SDK_VERSION}/MacOSX${SDK_VERSION}.sdk.tar.xz && \
    ./build.sh &&                                             \
    osxcross-macports install                                 \
	lua51                                                 \
	lua52                                                 \
	lua                                                   \
	openjdk8                                              \
	python27                                              \
	python35                                              \
	python36                                              \
	python37                                              \
	python38                                              \
	ruby24                                                \
	ruby25                                                \
	ruby26

# Install windows build tools (mingw) and libraries.
RUN mkdir -p /opt/wincross/archives                && \
    cd /opt/wincross                               && \
    mkdir java                                     && \
    wget -O archives/jdk8.zip https://github.com/AdoptOpenJDK/openjdk8-binaries/releases/download/jdk8u252-b09.1/OpenJDK8U-jdk_x86-32_windows_hotspot_8u252b09.zip && \
    unzip -d archives/java archives/jdk8.zip       && \
    mv archives/java/jdk*/* java                   && \
    mkdir lua51                                    && \
    wget -O archives/lua51.zip https://sourceforge.net/projects/luabinaries/files/5.1.5/Windows%20Libraries/Dynamic/lua-5.1.5_Win64_dllw6_lib.zip && \
    unzip -d lua51 archives/lua51.zip              && \
    mkdir lua52                                    && \
    wget -O archives/lua52.zip https://sourceforge.net/projects/luabinaries/files/5.2.4/Windows%20Libraries/Dynamic/lua-5.2.4_Win64_dllw6_lib.zip && \
    unzip -d lua52 archives/lua52.zip              && \
    mkdir lua53                                    && \
    wget -O archives/lua53.zip https://sourceforge.net/projects/luabinaries/files/5.3.5/Windows%20Libraries/Dynamic/lua-5.3.5_Win64_dllw6_lib.zip && \
    unzip -d lua53 archives/lua53.zip              && \
    rm -rf archives

# Run environment.
CMD /bin/bash
