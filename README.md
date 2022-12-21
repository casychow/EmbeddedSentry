# EmbeddedSentry
This is the code used for my Embedded Design class at NYU Tandon, taken Spring 2020.

To start, I find the average acceleration in the x, y, and z directions. This will be used to compare
the averages of the moving gyroscope.

When the MPU recognizes a direction based on changes in acceleration, it was assign a number
used to display the direction to the user.

After recording the sequence, the user will try to unlock the sentry.

If the recorded sequence is different than the attempted sequence, the sentry will
force the user to redo the sequence. If both sequences are the same, the sentry will
unlock.

YouTube video link: https://youtu.be/yOLNH1T1BG0
[![image](https://user-images.githubusercontent.com/58012214/208792638-328c7dc8-25d6-427b-9f85-5756649ee39f.png)](https://youtu.be/yOLNH1T1BG0)
