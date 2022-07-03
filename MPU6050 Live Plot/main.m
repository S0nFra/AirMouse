%% Copyright
% Course: Embedded Systems 2021/2022
%   
% Lecturer: Francesco Moscato	fmoscato@unisa.it
%  
% Group:
% 
% Ranieri Speranza		0622701687	s.ranieri4@studenti.unisa.it
% Mancusi Ernesto		0622701668	e.mancusi2@studenti.unisa.it
% Parrella Giuseppe		0622701665	g.parrella9@studenti.unisa.it
% Sonnessa Francesco		0622701672	f.sonnessa@studenti.unisa.it
% 
% Copyright (C) 2022 - All Rights Reserved
% This file is part of AirMouse.
% 
% AirMouse is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
% 
% AirMouse is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with AirMouse.  If not, see <http://www.gnu.org/licenses/>.
%%
clc
close all
clear all
%% vettore def
vect_fig = figure('Name','Mouse vector');
hv = quiver(0,0,0,1); %u,v
axis([-5 5 -5 5]);
vect = {1,hv};

% se non vuoi plottare il vettore di moviemento definisci
% vect = {0};

%% setup e inizio lettura
 if exist('vect','var') ~= 1
     vect = {0};
 end

[axes1, subplots1, sub_anim1] = create_plot();
[axes2, subplots2, sub_anim2] = create_plot();

figs = {{axes1, subplots1, sub_anim1};{axes2, subplots2, sub_anim2}};

global samples; samples=0;
s=serial('com3');
set(s,'BaudRate',115200);
s.BytesAvailableFcnMode = 'terminator';
s.BytesAvailableFcn=@(ser,event)serial_callback(ser,event,figs,vect);

%  Avvio del plot
fopen(s);

%% Termine
%Per terminare eseguire...
fclose(s); fclose(instrfind);
clc
disp('TERMINED');
