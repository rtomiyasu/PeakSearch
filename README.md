[日本語はこちら](https://github.com/rtomiyasu/PeakSearch/blob/main/README-ja.md)
# Instructions for PeakSearch CUI program

## Overview
[Version 0.99](https://github.com/rtomiyasu/PeakSearch/tree/main/PeakSearch0_9_99_win) is the initial release of the PeakSearch program.
The same program is also implemented in powder auto-indexing software Conograph GUI, and outputs an IGOR text file used for Conograph CUI.
The program executes peak-search for any kinds of 1-dimensional distributions (Figure 1) represented as a histogram data.

![Graph0](https://github.com/rtomiyasu/PeakSearch/assets/149344913/955ce5ea-31ed-41a0-85a7-79b028b8772f)
```
Figure 1 : Example of 1-dimensional distribution
```

Subsequently, an IGOR text file is outputted, which contains the powder diffraction pattern and the following values of peaks with the peak-height more than a given threshold ([Example](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample1(CharacteristicXrays)/output/sample1_pks.histogramIgor)):

1. peak-positions
1. peak-heights (estimated background is substracted from the y-value of the peaktop)
1. full-widths-at-half-maximum (FWHM)

If you open the file with IGOR Pro, you can see a plot as in Figure 2 immediately.

![Graph1](https://github.com/rtomiyasu/PeakSearch/assets/149344913/13c74545-501d-4ab6-ad24-64dc5f2056eb)
```
Figure 2 : Output IGOR text file
```

## FAQ
- [How to use the PeakSearch program](#How_to_use_the_PeakSearch_program)
- [How to adjust the parameters](#How_to_adjust_the_parameters)

### How_to_use_the_PeakSearch_program
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

### How_to_adjust_the_parameters
[Threshold](#Threshold), [NumberOfPointsForSGMethod](#NumberOfPointsForSGMethod) are necessary to be modified in order to obtain more successful results. It is also necessary to set [Alpha2Correction](#Alpha2Correction) for powder patterns including α2 reflections of characteristic X-rays.

#### Threshold
on peak-heights influences on peak-search results, because a peak at peak-position x is detected, if and only if it has a peak-height more than (Threshold) × Err[y], where Err[y] is the value of error in the y-value of the distribution at x. Our recommended value of Threshold is 3.0. Adjust Threshold (to a value within 1--10 if the input observational errors are proper), if the result is not satisfactory.

![Graph2](https://github.com/rtomiyasu/PeakSearch/assets/149344913/779f565e-93b7-4ae5-9a71-d3ce0f006e83)
```
Figure 3 : Example of a case of synchrotron data.
(By modifiying <Threshold>, it is possible to detect small diffraction peaks as well as peaks with high intensities.)
```

#### NumberOfPointsForSGMethod
It is also an important parameter so as not to pick up background noise as peaks.
If this number is smaller, the smoothing curve is fit to local irregularity more finely.
In Figure 4.1, noises are picked up as peaks, because the smoothing curve almost coincides with the background noise.

![Graph3_1](https://github.com/rtomiyasu/PeakSearch/assets/149344913/63407928-75fb-4e35-9a1b-f1024efe10e4)
![Graph3_2](https://github.com/rtomiyasu/PeakSearch/assets/149344913/9b2fa360-512a-45a3-b1d5-443f6b01a26a)
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

