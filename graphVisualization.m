data = importdata('Function_Zerg_Hydralisk.dat');
close all

index = 1:400;
for z = 1:5
    figure
    plot(index , data)
    grid
end