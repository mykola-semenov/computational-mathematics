Consider simple Hopf equation:

![Hopf equation](https://latex.codecogs.com/svg.latex?%5Cdfrac%7B%5Cpartial%20u%7D%7B%5Cpartial%20t%7D%20&plus;%20u%20%5Cdfrac%7B%5Cpartial%20u%7D%7B%5Cpartial%20x%7D%20%3D%200)

With initial conditions:

![initial conditions](http://latex.codecogs.com/svg.latex?%24%24%20u%28x%2C%200%29%20%3D%20%5Cphi%28x%29%20%5Cge%20%5Cvarepsilon%20%3E%200%20%24%24)

Divergent form:

![first divergent form](http://latex.codecogs.com/svg.latex?%24%24%20%5Cdfrac%7B%5Cpartial%20u%7D%7B%5Cpartial%20t%7D%20&plus;%20%5Cdfrac%7B%5Cpartial%7D%7B%5Cpartial%20x%7D%20%5Cdfrac%7Bu%5E2%7D%7B2%7D%20%3D%200%20%24%24)

Let ![v = u^2](http://latex.codecogs.com/svg.latex?%24v%20%3D%20u%5E2%24)

By multiplication by ![2 u](http://latex.codecogs.com/svg.latex?%242%20u%24) second divergent form can be got:

![second divergent form](http://latex.codecogs.com/svg.latex?%24%24%20%5Cdfrac%7B%5Cpartial%20u%5E2%7D%7B%5Cpartial%20t%7D%20&plus;%20%5Cdfrac%7B%5Cpartial%7D%7B%5Cpartial%20x%7D%20%5Cdfrac%7B2u%5E3%7D%7B3%7D%20%3D%20%5Cdfrac%7B%5Cpartial%20v%7D%7B%5Cpartial%20t%7D%20&plus;%20%5Cdfrac%7B2%7D%7B3%7D%20%5Cdfrac%7B%5Cpartial%20u%20v%7D%7B%5Cpartial%20x%7D%20%3D%20%5Cdfrac%7B%5Cpartial%20v%7D%7B%5Cpartial%20t%7D%20&plus;%20%5Cdfrac%7B2%20v%7D%7B3%7D%20%5Cdfrac%7B%5Cpartial%20u%7D%7B%5Cpartial%20x%7D%20&plus;%20%5Cdfrac%7B2%20u%7D%7B3%7D%20%5Cdfrac%7B%5Cpartial%20v%7D%7B%5Cpartial%20x%7D%20%3D%200%20%24%24)

Whole system:

![whole system](http://latex.codecogs.com/svg.latex?%24%24%20%5Cbegin%7Bcases%7D%20%5Cdfrac%7B%5Cpartial%20u%7D%7B%5Cpartial%20t%7D%20&plus;%20%5Cdfrac%7B%5Cpartial%7D%7B%5Cpartial%20x%7D%20%5Cdfrac%7Bu%5E2%7D%7B2%7D%20%3D%200%20%5C%5C%20%5C%5C%20%5Cdfrac%7B%5Cpartial%20v%7D%7B%5Cpartial%20t%7D%20&plus;%20%5Cdfrac%7B2%20v%7D%7B3%7D%20%5Cdfrac%7B%5Cpartial%20u%7D%7B%5Cpartial%20x%7D%20&plus;%20%5Cdfrac%7B2%20u%7D%7B3%7D%20%5Cdfrac%7B%5Cpartial%20v%7D%7B%5Cpartial%20x%7D%20%3D%200%20%5Cend%7Bcases%7D%20%24%24)

Or:

![matrix form](http://latex.codecogs.com/svg.latex?%24%24%20%5Cdfrac%7B%5Cpartial%7D%7B%5Cpartial%20t%7D%20%5Cbegin%7Bbmatrix%7D%20u%20%5C%5C%20v%20%5Cend%7Bbmatrix%7D%20&plus;%20A%20%5Cdfrac%7B%5Cpartial%7D%7B%5Cpartial%20x%7D%20%5Cbegin%7Bbmatrix%7D%20u%20%5C%5C%20v%20%5Cend%7Bbmatrix%7D%20%3D%20%5Cbegin%7Bbmatrix%7D%200%20%5C%5C%200%20%5Cend%7Bbmatrix%7D%20%24%24)

Where:

![A](http://latex.codecogs.com/svg.latex?%24%24%20A%20%3D%20%5Cbegin%7Bbmatrix%7D%200%20%26%20%5Cdfrac%7B1%7D%7B2%7D%20%5C%5C%20%5Cdfrac%7B2%20v%7D%7B3%7D%20%26%20%5Cdfrac%7B2%20u%7D%7B3%7D%20%5Cend%7Bbmatrix%7D%20%24%24)
