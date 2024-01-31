[to Japanese](https://github.com/rtomiyasu/PeakSearch/blob/main/README-ja.md)
# Instructions for PeakSearch CUI program

## Overview
[Version 0.99](https://github.com/rtomiyasu/PeakSearch/tree/main/PeakSearch0_9_99_win) is the initial release of the PeakSearch program.
The same program is also implemented in powder auto-indexing software [Conograph GUI](https://z-code-software.com/downloads/). The algorithm of the program is decribed [here](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearchAlgorithm.md).

The program executes peak-search for any kinds of 1-dimensional distributions (Figure 1) represented as a histogram data.

![Graph0](https://github.com/rtomiyasu/PeakSearch/assets/149344913/4c71a7c7-1d95-4a40-99c6-12da01b5950f)
```
Figure 1 : Example of 1-dimensional distribution
```

Subsequently, an IGOR text file is outputted, which contains the powder diffraction pattern and the following values of peaks with the peak-height more than a given threshold ([Example](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample1(CharacteristicXrays)/output/sample1_pks.histogramIgor)):

1. peak-positions
1. peak-heights (estimated background is substracted from the y-value of the peaktop)
1. full-widths-at-half-maximum (FWHM)

If you open the file with IGOR Pro, you can see a plot as in Figure 2 immediately.

![Graph1](https://github.com/rtomiyasu/PeakSearch/assets/149344913/63df6fe7-6696-4b3d-a585-fce109f7449c)
```
Figure 2 : Output IGOR text file
```

## NEWS
### 2024/1/31
- The web page of the source codes was moved from OSDN to GitHub.

## FAQ
- [How to use the PeakSearch program](#How_to_use_the_PeakSearch_program)
- [How to adjust the parameters](#How_to_adjust_the_parameters)
- [How do I report bugs?](#How_do_I_report_bugs)
- [Acknowledgments](#Acknowledgments)

### How to use the PeakSearch program
1. The PeakSearch program requires the three following input files. (Examples can be found in the "sample" folder.)
    - *.inp.xml file that includes information about input parameters ([Example](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample1(CharacteristicXrays)/sample1.inp.xml))
    - cntl.inp.xml file that includes the names of the *.inp.xml file and the output file ([Example](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample1(CharacteristicXrays)/cntl.inp.xml))
    - text file to give a 1-dimensional distribution. At present, the following three formats are available:
        - [XY](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample3(PF)/sample3.dat)
        - [IGOR](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample2(TOF)/sample2.histogramIgor)
        - [Rietan](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample1(CharacteristicXrays)/sample1.dat)
        - The errors of y-values in the third column of the XY and IGOR formats are used to compute a threshold. If the third column is empty, the square roots of y-values are used as the errors.
1. Copy one of the folders from the sample folder. Modify the contents of the two xml-files and the name of the *.inp.xml file if necessary. If you change the name of the *.inp.xml file, then it will be necessary to modify the contents of the cntl.inp.xml file accordingly.
1. Open a command prompt or terminal window in your operating system. Change the current folder to the same folder that contains the modified cntl.inp.xml file.
1. Enter the absolute path to the PeakSearch.exe file on the command line and execute PeakSearch.

### How to adjust the parameters
[Threshold](#Threshold), [NumberOfPointsForSGMethod](#NumberOfPointsForSGMethod) are necessary to be modified in order to obtain more successful results. It is also necessary to set [Alpha2Correction](#Alpha2Correction) for powder patterns including α2 reflections of characteristic X-rays.

#### Threshold
on peak-heights influences on peak-search results, because a peak at peak-position x is detected, if and only if it has a peak-height more than (Threshold) × Err[y], where Err[y] is the value of error in the y-value of the distribution at x. Our recommended value of Threshold is 3.0. Adjust Threshold (to a value within 1--10 if the input observational errors are proper), if the result is not satisfactory.

![Graph2](https://github.com/rtomiyasu/PeakSearch/assets/149344913/fd76b9a9-ddb2-4f53-8e38-d11a392b1f10)
```
Figure 3 : Example of a case of synchrotron data.
(By modifiying <Threshold>, it is possible to detect small diffraction peaks as well as peaks with high intensities.)
```

#### NumberOfPointsForSGMethod
It is also an important parameter so as not to pick up background noise as peaks.
If this number is smaller, the smoothing curve is fit to local irregularity more finely.
In Figure 4.1, noises are picked up as peaks, because the smoothing curve almost coincides with the background noise.

![Graph3_1](https://github.com/rtomiyasu/PeakSearch/assets/149344913/6455d667-fd3e-4e01-a5b6-ee52a357efcb)
![Graph3_2](https://github.com/rtomiyasu/PeakSearch/assets/149344913/e897d2c0-0021-4213-a4d3-fb987c55b50d)
```
Figure 4.1 : "NumberOfPointsForSGMethod" = 5	Figure 4.2 : "NumberOfPointsForSGMethod" = 25
```

#### Alpha2Correction
It should be set to 1 if the powder pattern contains α2 peaks, or set to 0 otherwise.
If it is not Cu Kα2, Kalpha1WaveLength and Kalpha2WaveLength are necessary to be modified accordingly.

| | Kα1  | Kα2  |
|:---:|---:|---:|
|Ag|0.5594075 |0.563798
|Co|1.788965  |1.792850
|Cr|2.289700  |2.293606
|Cu|1.540562  |1.544398
|Fe|1.936042  |1.939980
|Mo|0.709300  |0.713590

## How do I report bugs?
You should send us a bug report with all of the input and output files attached (including LOG_PEAKSEARCH.txt) to the following e-mail address:
- tomiyasu (at) imi.kyushu-u.ac.jp

## How do I cite the program?
If you use the program for your research, we strongly encourage you to include a citation of the following article in the bibliography.
- A. Esmaeili, T. Kamiyama, R. Oishi-Tomiyasu, "New functions and graphical user interface attached to powder indexing software", J. Appl. Cryst. 50 (2017), pp.651--659.

## Acknowledgments
I would like to express my gratitude to those who offered powder diffraction patterns for the Conograph project.

