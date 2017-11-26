# ThreadPool
A modern C++ `smart` thread pool.

It allows you to schedule jobs that can run only after a list of previously added jobs, like this

    //      0  1  2
    //      \  |  /
    //         3
    //      /  |  \
    //      4  5  6
    
In this case job #3 will run only after job 0, 1 and 2 are all finished.
Job 4, 5, 6 will run only after job #3 is finished.

Jobs do not require a template signature, as long as it returns void.
For cases where you need to schedule jobs that return...you can wrap them in a lambda that captures a reference (just beware of memory management). :)
