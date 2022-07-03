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
function [axes, subplots, sub_anim] = create_plot()
    figure();

    % Gx
    gx_sub = subplot(2,3,1); 
    title('G_x','FontSize',16);
    h_gx = animatedline('LineWidth',2);
    ylim([-200, 200]);

    % Gy
    gy_sub = subplot(2,3,2);
    title('G_y','FontSize',16);
    h_gy = animatedline('LineWidth',2);
    ylim([-200, 200]);

    % Gz
    gz_sub = subplot(2,3,3);
    title('G_z','FontSize',16);
    h_gz = animatedline('LineWidth',2);
    ylim([-200, 200]);

    % Ax
    ax_sub = subplot(2,3,4);
    title('A_x','FontSize',16);
    h_ax = animatedline('LineWidth',2);
    ylim([-20, 20]);

    % Ay
    ay_sub = subplot(2,3,5);
    title('A_y','FontSize',16);
    h_ay = animatedline('LineWidth',2);
    ylim([-20, 20]);

    % Az
    az_sub = subplot(2,3,6);
    title('A_z','FontSize',16);
    h_az = animatedline('LineWidth',2);
    ylim([-20, 20]);
    
    subplots.gx_sub = gx_sub;
    subplots.gy_sub = gy_sub;
    subplots.gz_sub = gz_sub;
    subplots.ax_sub = ax_sub;
    subplots.ay_sub = ay_sub;
    subplots.az_sub = az_sub;

    sub_anim.h_gx = h_gx;
    sub_anim.h_gy = h_gy;
    sub_anim.h_gz = h_gz;
    sub_anim.h_ax = h_ax;
    sub_anim.h_ay = h_ay;
    sub_anim.h_az = h_az;

    axes = gca;
end