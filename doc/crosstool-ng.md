# Mac os x$B$K%/%m%9%3%s%Q%$%k4D6-$r@0$($k(B

Mac os x$B$N(Bgcc$B$O(BELF$B$G$O$J$/!"(BMach-O$B$H$$$&%i%$%V%i%j7A<0$r;HMQ$9$k!#(B
$B$=$l$OJL$K$$$$$s$@$1$I(BLinux$B$G<B9T$G$-$k%P%$%J%j$r:n$j$?$$$H$-$K:$$k$N$G!"(BMac os X$B$K%/%m%9%3%s%Q%$%k4D6-$r@0Hw$9$k!#(B
$B%/%m%9%3%s%Q%$%k4D6-$N9=C[$O<jF0$G$b$G$-$k$i$7$$$N$@$,!"KM$O2?EY$b<:GT$7$F$7$^$C$?$N$G(BcrossTool-NG$B$H$$$&%S%k%I%D!<%k$r;HMQ$9$k$3$H$K$9$k!#(B
crosstool-ng : http://crosstool-ng.org/

## crosstool-NG$B$N%$%s%9%H!<%k(B

$B%=!<%9$r;}$C$F$/$k(B

   mkdir -p ~/local/src
   cd ~/local/src
   wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-1.9.3.tar.bz2
   tar jxf crosstool-ng-1.9.3.tar.bz2

gnu-sed,gnu-awk,automake,libtool(libtool,libtoolize),binutils(objcopy,objdump,readelf)$B$,I,MW$J$N$G(Bhomebrew$B$+$i%$%s%9%H!<%k$9$k(B

   brew install gnu-sed
   brew install gnu-awk
   brew install automake
   brew install libtool
   brew install binutils
   
$B%$%s%9%H!<%k@h$r:n$C$F!"%3%s%Q%$%k$9$k(B

   mkdir $HOME/local/cross
   ./configure --with-sed=/usr/local/Cellar/gnu-sed/4.2.1/bin/gsed --prefix=$HOME/local/cross --with-awk=/usr/local/Cellar/gawk/4.0.1/bin/gawk --with-libtool=/usr/local/Cellar/libtool/2.4.2/bin/glibtool --with-libtoolize=/usr/local/Cellar/libtool/2.4.2/bin/glibtoolize --with-objcopy=/usr/local/Cellar/binutils/2.22/bin/gobjcopy --with-objdump=/usr/local/Cellar/binutils/2.22/bin/gobjdump --with-readelf=/usr/local/Cellar/binutils/2.22/bin/greadelf
   make 
   make install

## $B%/%m%9%3%s%Q%$%kMQ$N@_Dj%U%!%$%k$r:n@.$9$k(B   

   mkdir -p $HOME/local/build_cross  
   cd $HOME/local/build_cross
   ct-ng menuconfig
 
kernel$B%3%s%U%#%0%l!<%7%g%s$N(Bmenuconfig$B$N$h$&$J2hLL$,3+$/$N$G!"%/%m%9%3%s%Q%$%k$7$?$$4D6-$K9g$o$;$F@_Dj$7$F$$$/!#(B
$B:#2s$NL\E*$O(Bmac os x$B$G(Bcentos$BMQ$N%P%$%J%j$rEG$/$3$H$J$N$G!"<!$N$h$&$K@_Dj$rJQ99$7$?!#(B
$BCm0U$7$J$$$H$$$1$J$$$N$O!"(BC library$B$O(Bglibc$B$r;HMQ$7!"(BCloog/ppl$B$N%P!<%8%g%s$O(B0.15.10$B$O%3%s%Q%$%k$,<:GT$7$?!#(B

   target options -> Target Architecture -> x86
                     Bitness -> 32-bits
   Operationg System -> Target OS -> linux
   C-library -> C library -> glibc
   Companion libraries -> Cloog/ppl version -> 0.15.9

## $B%/%m%9%3%s%Q%$%i$r%S%k%I$9$k(B

$B%3%s%U%#%0$r:n@.$7$?$i$=$l$K4p$E$$$F%S%k%I$9$k(B

   ct-ng build
   
Mountain Lion$B$G;n$7$?$i(Byour file system *not* case-sensitive$B$C$F8@$o$l$F<:GT$9$k!#!#!#(B
$BJL$N%U%!%$%k%7%9%F%`$r:n$k$7$+$J$$$N$+!)(B
