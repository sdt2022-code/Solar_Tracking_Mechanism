%% Beirut,Lebanon 
% Location Data 
Bei_location.latitude = 33.88863; 
Bei_location.longitude = 35.495480; 
Bei_location.altitude = 19; 

% Invariant Time Data 
Bei_time.UTC = 2;
Bei_time.year = 2022; 
Bei_time.min = 0; 
Bei_time.sec = 0;


%% Summer Solstice: June 21  
Bei_time.month = 6;
Bei_time.day = 21;
Bei_azimuth_sum = zeros(24,1); 
Bei_zenith_sum = Bei_azimuth_sum;
for t = 1:24
    Bei_time.hour = t-1; 
    sun = sun_position(Bei_time, Bei_location); 
    Bei_azimuth_sum(t) = sun.azimuth;  
    Bei_zenith_sum(t) = sun.zenith;  
end

%% Winter Solstice: December 21  
Bei_time.month = 12;
Bei_time.day = 21;
Bei_azimuth_win = zeros(24,1); 
Bei_zenith_win = Bei_azimuth_win;
for t = 1:24
    Bei_time.hour = t-1; 
    sun = sun_position(Bei_time, Bei_location); 
    Bei_azimuth_win(t) = sun.azimuth;  
    Bei_zenith_win(t) = sun.zenith;  
end

%% Vernal Equinox: March 21  
Bei_time.month = 3;
Bei_time.day = 21;
Bei_azimuth_ver = zeros(24,1); 
Bei_zenith_ver = Bei_azimuth_ver;
for t = 1:24
    Bei_time.hour = t-1; 
    sun = sun_position(Bei_time, Bei_location); 
    Bei_azimuth_ver(t) = sun.azimuth;  
    Bei_zenith_ver(t) = sun.zenith;  
end

%% Plot Results 
T = 0:23; 
figure(1);  
plot(T,Bei_azimuth_sum);
grid on;
hold on; 
plot(T,Bei_azimuth_win);
plot(T,Bei_azimuth_ver);
title('Solar Azimuth Angle in Different Seasons'); 
xlabel('Time of Day (h)'); 
ylabel('Azimuth Angle (degrees)'); 
legend('Summer Solstice','Winter Solstice','Vernal Equinox'); 

figure(2); 
plot(T,Bei_zenith_sum);
grid on;
hold on; 
plot(T,Bei_zenith_win);
plot(T,Bei_zenith_ver);
title('Solar Zenith Angle in Different Seasons'); 
xlabel('Time of Day (h)'); 
ylabel('Zenith Angle (degrees)'); 
legend('Summer Solstice','Winter Solstice','Vernal Equinox'); 
