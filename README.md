# Introduction #
This software performs part of speech tagging, word segmentation, and
phoneme analysis for Vietnamese [(Homepage)](http://www.truongdo.com/vita).

Basically, the toolkit solves the tasks by appling CRFs method that implemented
in [CRFSuite](http://www.chokkan.org/software/crfsuite/)

This has been tested on __Ubuntu 14.04 lts__.

# Dependencies #
In order to compile the program, you need to install the following software:

- boost: `sudo apt-get install libboost-all-dev`
- cmake: `sudo apt-get install cmake`

The script `install_depend.sh` will automatically install __CRFSuite__ and __liblbfgs-1.10__.

# Installation #
1. Install boost C++
2. ./install_depend.sh
3. cd build && cmake ../src && make

# Usages #
## Model downloads ##
The model and also the training scripts can be found in [vita-model](https://github.com/truongdo/vita-model)

## Program: `vita` ##

PoS tagging, word segmentation, dictionary generates

``echo "Hai nghi phạm Nguyễn Hải Dương và Vũ Văn Tiến" | vita -m model_dir``

Output:
```
Hai   M,B-NP,0,h a iz
nghi_phạm   M,I-NP,0_5,ng i_ph a mz
Nguyễn_Hải_Dương    Nu,I-NP,2_3_0,ng w ie nz_h a iz_d wa ngz
và    Cc,B-VP,1,v a
Vũ_Văn_Tiến   V,I-VP,2_0_4,v u_v aw nz_t ie nz
```

Output format:
``word    PoS,chunking info,tone(s),phoneme(s)``

Run `vita -h` for more options.

## Program: `vtalk_ana` ##

Phoneme analysys (mainly used for text-to-speech)
``echo "Hai nghi phạm Nguyễn Hải Dương và Vũ Văn Tiến" | vita_ana -m model_dir``

Output:
```
xx^xx-sil+h=a@0-0/A:xx_0/B:xx-1@0-0&0-0/C:0+3/D:xx-0/E:xx-1/F:M-3/G:0-0/H:1=1@0=3/I:18-3/J:30+5-2
xx^sil-h+a=iz@0-2/A:xx_1/B:0-3@0-0&0-2/C:0+2/D:xx-1/E:M-3/F:M-5/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
sil^h-a+iz=ng@1-1/A:xx_1/B:0-3@0-0&0-2/C:0+2/D:xx-1/E:M-3/F:M-5/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
h^a-iz+ng=i@2-0/A:xx_1/B:0-3@0-0&0-2/C:0+2/D:xx-1/E:M-3/F:M-5/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
a^iz-ng+i=ph@0-1/A:0_3/B:0-2@0-1&1-2/C:5+3/D:M-3/E:M-5/F:Nu-10/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
iz^ng-i+ph=a@1-0/A:0_3/B:0-2@0-1&1-2/C:5+3/D:M-3/E:M-5/F:Nu-10/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
ng^i-ph+a=mz@0-2/A:0_2/B:5-3@1-0&2-1/C:2+4/D:M-3/E:M-5/F:Nu-10/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
i^ph-a+mz=ng@1-1/A:0_2/B:5-3@1-0&2-1/C:2+4/D:M-3/E:M-5/F:Nu-10/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
ph^a-mz+ng=w@2-0/A:0_2/B:5-3@1-0&2-1/C:2+4/D:M-3/E:M-5/F:Nu-10/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
a^mz-ng+w=ie@0-3/A:5_3/B:2-4@0-2&2-2/C:3+3/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
mz^ng-w+ie=nz@1-2/A:5_3/B:2-4@0-2&2-2/C:3+3/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
ng^w-ie+nz=h@2-1/A:5_3/B:2-4@0-2&2-2/C:3+3/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
w^ie-nz+h=a@3-0/A:5_3/B:2-4@0-2&2-2/C:3+3/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
ie^nz-h+a=iz@0-2/A:2_4/B:3-3@1-1&3-1/C:0+3/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
nz^h-a+iz=d@1-1/A:2_4/B:3-3@1-1&3-1/C:0+3/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
h^a-iz+d=wa@2-0/A:2_4/B:3-3@1-1&3-1/C:0+3/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
a^iz-d+wa=ngz@0-2/A:3_3/B:0-3@2-0&4-0/C:1+2/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
iz^d-wa+ngz=v@1-1/A:3_3/B:0-3@2-0&4-0/C:1+2/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
d^wa-ngz+v=a@2-0/A:3_3/B:0-3@2-0&4-0/C:1+2/D:M-5/E:Nu-10/F:Cc-2/G:1-1/H:18=3@1=2/I:10-2/J:30+5-2
wa^ngz-v+a=v@0-1/A:0_3/B:1-2@0-0&0-1/C:2+2/D:Nu-10/E:Cc-2/F:V-8/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
ngz^v-a+v=u@1-0/A:0_3/B:1-2@0-0&0-1/C:2+2/D:Nu-10/E:Cc-2/F:V-8/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
v^a-v+u=v@0-1/A:1_2/B:2-2@0-2&1-2/C:0+3/D:Cc-2/E:V-8/F:xx-1/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
a^v-u+v=aw@1-0/A:1_2/B:2-2@0-2&1-2/C:0+3/D:Cc-2/E:V-8/F:xx-1/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
v^u-v+aw=nz@0-2/A:2_2/B:0-3@1-1&2-1/C:4+3/D:Cc-2/E:V-8/F:xx-1/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
u^v-aw+nz=t@1-1/A:2_2/B:0-3@1-1&2-1/C:4+3/D:Cc-2/E:V-8/F:xx-1/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
v^aw-nz+t=ie@2-0/A:2_2/B:0-3@1-1&2-1/C:4+3/D:Cc-2/E:V-8/F:xx-1/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
aw^nz-t+ie=nz@0-2/A:0_3/B:4-3@2-0&3-0/C:xx+1/D:Cc-2/E:V-8/F:xx-1/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
nz^t-ie+nz=sil@1-1/A:0_3/B:4-3@2-0&3-0/C:xx+1/D:Cc-2/E:V-8/F:xx-1/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
t^ie-nz+sil=xx@2-0/A:0_3/B:4-3@2-0&3-0/C:xx+1/D:Cc-2/E:V-8/F:xx-1/G:18-3/H:10=2@2=1/I:1-1/J:30+5-2
ie^nz-sil+xx=xx@0-0/A:4_3/B:xx-1@0-0&0-0/C:xx+0/D:V-8/E:xx-1/F:xx-0/G:10-2/H:1=1@3=0/I:0-0/J:30+5-2
```

Output format:
Please read `doc/lab_format.pdf`

# Citation #
Please use the following Bibtex when you want to cite `vita`:
```
  @misc{truong_vita,
    author = {Quoc Truong Do},
    title = {Vita: A Toolkit for Vietnamese segmentation, chunking, part of speech tagging and morphological analyzer},
    url = {http://truongdo.com/vita/},
    year = {2015}
  }
```

# TODO #

# Credits #
[CRFSuite](http://www.chokkan.org/software/crfsuite/)

