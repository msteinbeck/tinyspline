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
        doxygen                                \
        dub                                    \
        file                                   \
        flex                                   \
        gcc-arm-linux-gnueabihf                \
        git                                    \
        graphviz                               \
        lcov                                   \
        libtool                                \
        libfltk1.3-dev fluid                   \
        libpcre2-dev                           \
        lua5.1 liblua5.1-dev                   \
        lua5.2 liblua5.2-dev                   \
        lua5.3 liblua5.3-dev                   \
        luarocks                               \
        maven                                  \
        mingw-w64                              \
        mono-mcs mono-reference-assemblies-*   \
        nuget                                  \
        octave liboctave-dev                   \
        php-dev                                \
        pkg-config                             \
        python python-dev python-pip           \
        python3-dev python3-pip                \
        p7zip-full                             \
        r-base r-cran-rcpp                     \
        ruby ruby-dev                          \
        texinfo                                \
        wget                                   \
        zip &&                                 \
    python2 -m pip install setuptools wheel && \
    python3 -m pip install setuptools wheel && \
    wget -P /usr/local/src/doxygen \
        https://sourceforge.net/projects/doxygen/files/rel-1.9.0/doxygen-1.9.0.src.tar.gz && \
    tar -xf /usr/local/src/doxygen/doxygen-1.9.0.src.tar.gz \
        -C /usr/local/src/doxygen \
        --strip 1 && \
    cd /usr/local/src/doxygen && \
        cmake . && \
        make && \
        make install && \
    wget -P /usr/local/src/swig \
        http://prdownloads.sourceforge.net/swig/swig-4.1.1.tar.gz && \
    tar -xf /usr/local/src/swig/swig-4.1.1.tar.gz \
        -C /usr/local/src/swig \
        --strip 1 && \
    cd /usr/local/src/swig && \
        ./autogen.sh && \
        ./configure && \
        make && \
        make install && \
    wget -P /usr/local/src/cmake \
        https://github.com/Kitware/CMake/releases/download/v3.22.3/cmake-3.22.3-linux-x86_64.tar.gz && \
    tar -xf /usr/local/src/cmake/cmake-3.22.3-linux-x86_64.tar.gz \
        -C /usr/local \
        --strip 1 && \
    git clone https://github.com/emscripten-core/emsdk.git /opt/emsdk && \
    cd /opt/emsdk && \
        ./emsdk install 2.0.20 && \
        ./emsdk activate 2.0.20 && \
        echo "source $(pwd)/emsdk_env.sh" >> $HOME/.bashrc && \
    mkdir -p /opt/android && \
    wget -P /usr/local/src/ndk23b \
        https://dl.google.com/android/repository/android-ndk-r23b-linux.zip && \
    unzip -d /opt/android /usr/local/src/ndk23b/android-ndk-r23b-linux.zip && \
    wget -P /usr/local/src/python37 \
        https://www.python.org/ftp/python/3.7.9/Python-3.7.9.tar.xz && \
    tar -xf /usr/local/src/python37/Python-3.7.9.tar.xz \
        -C /usr/local/src/python37 \
        --strip 1 && \
    cd /usr/local/src/python37 && \
        ./configure \
            --prefix=/opt/linux/python37 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/python37/lib && \
        make && \
        make install && \
        /opt/linux/python37/bin/python3 -m pip install wheel && \
    wget -P /usr/local/src/python38 \
        https://www.python.org/ftp/python/3.8.6/Python-3.8.6.tar.xz && \
    tar -xf /usr/local/src/python38/Python-3.8.6.tar.xz \
        -C /usr/local/src/python38 \
        --strip 1 && \
    cd /usr/local/src/python38 && \
        ./configure \
            --prefix=/opt/linux/python38 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/python38/lib && \
        make && \
        make install && \
        /opt/linux/python38/bin/python3 -m pip install wheel && \
    wget -P /usr/local/src/python39 \
        https://www.python.org/ftp/python/3.9.0/Python-3.9.0.tar.xz && \
    tar -xf /usr/local/src/python39/Python-3.9.0.tar.xz \
        -C /usr/local/src/python39 \
        --strip 1 && \
    cd /usr/local/src/python39 && \
        ./configure \
            --prefix=/opt/linux/python39 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/python39/lib && \
        make && \
        make install && \
        /opt/linux/python39/bin/python3 -m pip install wheel && \
    wget -P /usr/local/src/python310 \
        https://www.python.org/ftp/python/3.10.2/Python-3.10.2.tar.xz && \
    tar -xf /usr/local/src/python310/Python-3.10.2.tar.xz \
        -C /usr/local/src/python310 \
        --strip 1 && \
    cd /usr/local/src/python310 && \
        ./configure \
            --prefix=/opt/linux/python310 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/python310/lib && \
        make && \
        make install && \
        wget https://files.pythonhosted.org/packages/27/d6/003e593296a85fd6ed616ed962795b2f87709c3eee2bca4f6d0fe55c6d00/wheel-0.37.1-py2.py3-none-any.whl && \
        /opt/linux/python310/bin/python3 -m pip install wheel-0.37.1-py2.py3-none-any.whl && \
    wget -P /usr/local/src/python311 \
        https://www.python.org/ftp/python/3.11.1/Python-3.11.1.tar.xz && \
    tar -xf /usr/local/src/python311/Python-3.11.1.tar.xz \
        -C /usr/local/src/python311 \
        --strip 1 && \
    cd /usr/local/src/python311 && \
        ./configure \
            --prefix=/opt/linux/python311 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/python311/lib && \
        make && \
        make install && \
        wget https://files.pythonhosted.org/packages/27/d6/003e593296a85fd6ed616ed962795b2f87709c3eee2bca4f6d0fe55c6d00/wheel-0.37.1-py2.py3-none-any.whl && \
        /opt/linux/python311/bin/python3 -m pip install wheel-0.37.1-py2.py3-none-any.whl && \
    wget -P /usr/local/src/ruby24 \
        https://cache.ruby-lang.org/pub/ruby/2.4/ruby-2.4.10.tar.gz && \
    tar -xf /usr/local/src/ruby24/ruby-2.4.10.tar.gz \
        -C /usr/local/src/ruby24 \
        --strip 1 && \
    cd /usr/local/src/ruby24 && \
        ./configure \
            --prefix=/opt/linux/ruby24 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/ruby24/lib && \
        make && \
        make install && \
    wget -P /usr/local/src/ruby25 \
        https://cache.ruby-lang.org/pub/ruby/2.5/ruby-2.5.8.tar.gz && \
    tar -xf /usr/local/src/ruby25/ruby-2.5.8.tar.gz \
        -C /usr/local/src/ruby25 \
        --strip 1 && \
    cd /usr/local/src/ruby25 && \
        ./configure \
            --prefix=/opt/linux/ruby25 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/ruby25/lib && \
        make && \
        make install && \
    wget -P /usr/local/src/ruby26 \
        https://cache.ruby-lang.org/pub/ruby/2.6/ruby-2.6.6.tar.gz && \
    tar -xf /usr/local/src/ruby26/ruby-2.6.6.tar.gz \
        -C /usr/local/src/ruby26 \
        --strip 1 && \
    cd /usr/local/src/ruby26 && \
        ./configure \
            --prefix=/opt/linux/ruby26 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/ruby26/lib && \
        make && \
        make install && \
    wget -P /usr/local/src/ruby27 \
        https://cache.ruby-lang.org/pub/ruby/2.7/ruby-2.7.2.tar.gz && \
    tar -xf /usr/local/src/ruby27/ruby-2.7.2.tar.gz \
        -C /usr/local/src/ruby27 \
        --strip 1 && \
    cd /usr/local/src/ruby27 && \
        ./configure \
            --prefix=/opt/linux/ruby27 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/ruby27/lib && \
        make && \
        make install && \
    wget -P /usr/local/src/ruby30 \
        https://cache.ruby-lang.org/pub/ruby/3.0/ruby-3.0.4.tar.gz && \
    tar -xf /usr/local/src/ruby30/ruby-3.0.4.tar.gz \
        -C /usr/local/src/ruby30 \
        --strip 1 && \
    cd /usr/local/src/ruby30 && \
        ./configure \
            --prefix=/opt/linux/ruby30 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/ruby30/lib && \
        make && \
        make install && \
    wget -P /usr/local/src/ruby31 \
        https://cache.ruby-lang.org/pub/ruby/3.1/ruby-3.1.2.tar.gz && \
    tar -xf /usr/local/src/ruby31/ruby-3.1.2.tar.gz \
        -C /usr/local/src/ruby31 \
        --strip 1 && \
    cd /usr/local/src/ruby31 && \
        ./configure \
            --prefix=/opt/linux/ruby31 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/ruby31/lib && \
        make && \
        make install && \
    wget -P /usr/local/src/ruby32 \
        https://cache.ruby-lang.org/pub/ruby/3.2/ruby-3.2.0.tar.gz && \
    tar -xf /usr/local/src/ruby32/ruby-3.2.0.tar.gz \
        -C /usr/local/src/ruby32 \
        --strip 1 && \
    cd /usr/local/src/ruby32 && \
        ./configure \
            --prefix=/opt/linux/ruby32 \
            --enable-shared \
            LDFLAGS=-Wl,-rpath=/opt/linux/ruby32/lib && \
        make && \
        make install && \
    wget -P /usr/local/src/lua54 \
        https://sourceforge.net/projects/luabinaries/files/5.4.2/Linux%20Libraries/lua-5.4.2_Linux54_64_lib.tar.gz && \
    mkdir -p /opt/linux/lua54 && \
    tar -xf /usr/local/src/lua54/lua-5.4.2_Linux54_64_lib.tar.gz \
        -C /opt/linux/lua54 && \
    cd /usr/local/src && rm -rf ..?* .[!.]* *

# Install windows build tools (mingw) and libraries.
COPY toolchain-mingw64.cmake /tmp
RUN mkdir -p /opt/windows-x86_64/archives                       && \
    mv /tmp/toolchain-mingw64.cmake /opt/windows-x86_64         && \
    cd /opt/windows-x86_64                                      && \
    wget -O archives/fltk13.tar.gz https://github.com/fltk/fltk/archive/refs/tags/release-1.3.8.tar.gz && \
    mkdir archives/fltk                                         && \
    tar -xf archives/fltk13.tar.gz -C archives/fltk --strip 1   && \
    cd archives/fltk                                            && \
        cmake -DCMAKE_TOOLCHAIN_FILE=/opt/windows-x86_64/toolchain-mingw64.cmake \
              -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 \
              -DFLTK_BUILD_TEST=OFF .                           && \
        make install                                            && \
        cd /opt/windows-x86_64                                  && \
    mkdir java                                                  && \
    wget -O archives/jdk8.zip https://github.com/AdoptOpenJDK/openjdk8-binaries/releases/download/jdk8u252-b09.1/OpenJDK8U-jdk_x86-32_windows_hotspot_8u252b09.zip && \
    unzip -d archives/java archives/jdk8.zip                    && \
    mv archives/java/jdk*/* java                                && \
    ln -s /opt/windows-x86_64/java /usr/x86_64-w64-mingw32/java && \
    mkdir lua51                                                 && \
    wget -O archives/lua51.zip https://sourceforge.net/projects/luabinaries/files/5.1.5/Windows%20Libraries/Dynamic/lua-5.1.5_Win64_dllw6_lib.zip && \
    unzip -d lua51 archives/lua51.zip                           && \
    mkdir lua52                                                 && \
    wget -O archives/lua52.zip https://sourceforge.net/projects/luabinaries/files/5.2.4/Windows%20Libraries/Dynamic/lua-5.2.4_Win64_dllw6_lib.zip && \
    unzip -d lua52 archives/lua52.zip                           && \
    mkdir lua53                                                 && \
    wget -O archives/lua53.zip https://sourceforge.net/projects/luabinaries/files/5.3.6/Windows%20Libraries/Dynamic/lua-5.3.6_Win64_dllw6_lib.zip && \
    unzip -d lua53 archives/lua53.zip                           && \
    mkdir lua54                                                 && \
    wget -O archives/lua54.zip https://sourceforge.net/projects/luabinaries/files/5.4.2/Windows%20Libraries/Dynamic/lua-5.4.2_Win64_dllw6_lib.zip && \
    unzip -d lua54 archives/lua54.zip                           && \
    wget -O archives/ruby26.7z https://github.com/oneclick/rubyinstaller2/releases/download/RubyInstaller-2.6.10-1/rubyinstaller-2.6.10-1-x64.7z && \
    7z x archives/ruby26.7z -oruby26                            && \
    cd ruby26/ruby* && mv * .. && cd ../..                      && \
    wget -O archives/ruby27.7z https://github.com/oneclick/rubyinstaller2/releases/download/RubyInstaller-2.7.6-1/rubyinstaller-2.7.6-1-x64.7z && \
    7z x archives/ruby27.7z -oruby27                            && \
    cd ruby27/ruby* && mv * .. && cd ../..                      && \
    wget -O archives/ruby30.7z https://github.com/oneclick/rubyinstaller2/releases/download/RubyInstaller-3.0.4-1/rubyinstaller-3.0.4-1-x64.7z && \
    7z x archives/ruby30.7z -oruby30                            && \
    cd ruby30/ruby* && mv * .. && cd ../..                      && \
    wget -O archives/ruby31.7z https://github.com/oneclick/rubyinstaller2/releases/download/RubyInstaller-3.1.2-1/rubyinstaller-3.1.2-1-x64.7z && \
    7z x archives/ruby31.7z -oruby31                            && \
    cd ruby31/ruby* && mv * .. && cd ../..                      && \
    wget -O archives/ruby32.7z https://github.com/oneclick/rubyinstaller2/releases/download/RubyInstaller-3.2.0-1/rubyinstaller-3.2.0-1-x64.7z && \
    7z x archives/ruby32.7z -oruby32                            && \
    cd ruby32/ruby* && mv * .. && cd ../..                      && \
    rm -rf archives

# Run environment.
CMD /bin/bash
