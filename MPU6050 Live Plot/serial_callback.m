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
function serial_callback(ser,~,figs,vect)
    % this is the entire string
    val_raw = fscanf(ser)
    % disp(val_raw);
    MPUs_val = split(val_raw,';');

    if size(MPUs_val,1) ~= size(figs,1)
        ME = MException('MyComponent:noSuchVariable', ...
        'I dati sulla seriale sono per %d MPU, passati %d',size(MPUs_val,1), size(figs,1));
        throw(ME)
    end

    global samples;

    % MPU1
    values1 = split(MPUs_val{1},',');
    gx1 =  str2double(values1{1});
    gy1 = str2double(values1{2});
    gz1 =  str2double(values1{3});
    ax1 = str2double(values1{4});
    ay1 = str2double(values1{5});
    az1 = str2double(values1{6});

    axes1 = figs{1}{1};
    subplots1 = figs{1}{2};
    sub_anim1 = figs{1}{3};
    
    addpoints(sub_anim1.h_gx, samples, gx1);
    addpoints(sub_anim1.h_gy, samples, gy1);
    addpoints(sub_anim1.h_gz, samples, gz1);
    addpoints(sub_anim1.h_ax, samples, ax1);
    addpoints(sub_anim1.h_ay, samples, ay1);
    addpoints(sub_anim1.h_az, samples, az1);
    drawnow update;

    if mod(samples,20) == 0
        xlim(axes1, [samples-5,samples + 40]);
        linkaxes([subplots1.gx_sub, ...
            subplots1.gy_sub, ...
            subplots1.gz_sub, ...
            subplots1.ax_sub, ...
            subplots1.ay_sub, ...
            subplots1.az_sub],'x');
    end

    % MPU2
    values2 = split(MPUs_val{2},',');
    gx2 =  str2double(values2{1});
    gy2 = str2double(values2{2});
    gz2 =  str2double(values2{3});
    ax2 = str2double(values2{4});
    ay2 = str2double(values2{5});
    az2 = str2double(values2{6});

    axes2 = figs{2}{1};
    subplots2 = figs{2}{2};
    sub_anim2 = figs{2}{3};
    
    addpoints(sub_anim2.h_gx, samples, gx2);
    addpoints(sub_anim2.h_gy, samples, gy2);
    addpoints(sub_anim2.h_gz, samples, gz2);
    addpoints(sub_anim2.h_ax, samples, ax2);
    addpoints(sub_anim2.h_ay, samples, ay2);
    addpoints(sub_anim2.h_az, samples, az2);
    drawnow update;

    samples = samples+1;

    if mod(samples,20) == 0
        xlim(axes2, [samples-5,samples + 40]);
        linkaxes([subplots2.gx_sub, ...
            subplots2.gy_sub, ...
            subplots2.gz_sub, ...
            subplots2.ax_sub, ...
            subplots2.ay_sub, ...
            subplots2.az_sub],'x');
    end

    % Vettore
    if vect{1} == 1
        vect{2}.UData= -ax1;
        vect{2}.VData = ay1;
    else
        vect{2}.UData= -ax2;
        vect{2}.VData = ay2;
    end
    
    % [ax1,ay1,az1,gx1,gy1,gz1;ax1,ay2,az2,gx2,gy2,gz2];
    current_data.mpu1.ax = ax1;
    current_data.mpu1.ay = ay1;
    current_data.mpu1.az = az1;
    current_data.mpu1.gx = gx1;
    current_data.mpu1.gy = gy1;
    current_data.mpu1.gz = gz1;

    current_data.mpu2.ax = ax2;
    current_data.mpu2.ay = ay2;
    current_data.mpu2.az = az2;
    current_data.mpu2.gx = gx2;
    current_data.mpu2.gy = gy2;
    current_data.mpu2.gz = gz2;

    result = checkThis(current_data);
    global i;
    if (result{1} ~= -1)
        disp([result{2} i])
        i=i+1;
        if mod(samples,20) == 0
            clc
        end
    end

end