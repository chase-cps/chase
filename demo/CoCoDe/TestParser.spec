constant Integer c: 5;

controllable variable x is:
    Integer in [3:9);

controllable variable p is:
        Real in (0:1),
        parametric;

distribution D is:
    gaussian(mu = 1, sigma = p + 0.1);