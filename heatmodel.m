clc;clear;close all

HTmodel = createpde('thermal','steadystate');
g = decsg([3 4 0 0 0.3048 0.3048 -0.3048 0.3048 0.3048 -0.3048]');
geometryFromEdges(HTmodel,g);

figure
pdegplot(HTmodel,"EdgeLabels","on")
axis equal
k = 40; % Thermal conductivity, W/(m*C)
rho = 7800; % Density, kg/m^3
cp = 500; % Specific heat, W*s/(kg*C)
q = 20000; % Heat source, W/m^3

thermalProperties(HTmodel,"ThermalConductivity",k);
internalHeatSource(HTmodel,q);
thermalBC(HTmodel,"Edge",2,"Temperature",0);
thermalBC(HTmodel,"Edge",1,"Temperature",100);
thermalBC(HTmodel,"Edge",4,"Temperature",0);
thermalBC(HTmodel,"Edge",3,...
                       "ConvectionCoefficient",25.32,...
                       "AmbientTemperature",33);
msh = generateMesh(HTmodel);
figure
pdeplot(HTmodel)
axis equal
result = solve(HTmodel);
T = result.Temperature;

figure
pdeplot(HTmodel,"XYData",T,"Contour","on")
axis equal
title("Steady-State Temperature")