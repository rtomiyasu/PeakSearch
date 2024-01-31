# Peak Search Algorithm
Our peak search program adopts a method that does not require any kind of prior processing such as smoothing and background subtraction.
The following are parameters of a peak:

- $`x_0`$ : peak position,
- $`h`$ : peak height,
- $`FWHM`$ : full width at half maximum.

The following 1--4 explain all the steps of the algorithm:

1. Compute the second derivative of the input x-y plot data, by using a cubic polynomial that approximates the x-y plot locally.
1. Obtain all the local minimum points of the second derivative with a negative y-value. Approximate the area between the x-axis and the graph of the second derivative (A in the following figure):

    ![figure1_3](https://github.com/rtomiyasu/PeakSearch/assets/149344913/7c2d3a03-7f9c-4dcf-823a-5783c98750c1)

    ```
    Figure 1 : Example of the second derivative of a 1-dimensional Guaussian-like peak.
    ```

1. Using the following formula, estimate the peak height h at every local minimum point $`x_0`$ ("peak height" means the "y-value of the peaktop" minus "estimated background value"):

    $`h=\dfrac{e^{\dfrac{1}{2}}}{4}A(x_2-x_1)`$&emsp;&emsp;&emsp;(Eq.1)

    where $`x_1`$ and $`x_2`$ are the x-coordinates at which the second derivative equals zero (Figure 1).

1. If the above h is larger than the threshold determined from the input parameters, a peak with the height h exists at $`x_0`$. Estimate the FWHM using the following formula:

    $`FWHM={\sqrt{2\ln{2}}}(x_2-x_1)`$&emsp;&emsp;&emsp;(Eq.2)

1. Eq. 1 and Eq. 2 are the equations any 1-dimensional Gaussian function $`f(x)=he^{-\dfrac{{(x-x_0)}^2}{2σ^2}}`$ satisfies.
