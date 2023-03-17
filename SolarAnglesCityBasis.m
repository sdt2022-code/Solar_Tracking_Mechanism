%% Beirut,Lebanon 
% Location Data 
Bei_location.latitude = 33.88863; 
Bei_location.longitude = 35.495480; 
Bei_location.altitude = 19; 

% Invariant Time Data 
Bei_time.UTC = 2;
Bei_time.year = 2021; 
Bei_time.month = 6;
Bei_time.day = 21;
Bei_time.min = 0; 
Bei_time.sec = 0;

Bei_azimuth = zeros(24,1); 
Bei_zenith = Bei_azimuth; 

%% Reykjavik,Iceland  
% Location Data 
Rey_location.latitude = 64.128288; 
Rey_location.longitude = -21.827774; 
Rey_location.altitude = 0; 

% Invariant Time Data 
Rey_time.UTC = 0;
Rey_time.year = 2021; 
Rey_time.month = 6;
Rey_time.day = 21;
Rey_time.min = 0; 
Rey_time.sec = 0;

Rey_azimuth = zeros(24,1); 
Rey_zenith = Rey_azimuth; 

%% Quito,Ecuador 
% Location Data 
Qui_location.latitude = -0.180653; 
Qui_location.longitude = -78.467834; 
Qui_location.altitude = 2850; 

% Invariant Time Data 
Qui_time.UTC = -5;
Qui_time.year = 2021; 
Qui_time.month = 6;
Qui_time.day = 21;
Qui_time.min = 0; 
Qui_time.sec = 0;

Qui_azimuth = zeros(24,1); 
Qui_zenith = Qui_azimuth; 

%% Longyearbyen, Norway 
% Location Data 
Lon_location.latitude = 78.2232; 
Lon_location.longitude = 15.6267; 
Lon_location.altitude = 1717; 

% Invariant Time Data 
Lon_time.UTC = 1;
Lon_time.year = 2021; 
Lon_time.month = 6;
Lon_time.day = 21;
Lon_time.min = 0; 
Lon_time.sec = 0;

Lon_azimuth = zeros(24,1); 
Lon_zenith = Lon_azimuth;

%% Copenhagen, Norway 
% Location Data 
Cop_location.latitude = 55.6761; 
Cop_location.longitude = 12.5683; 
Cop_location.altitude = 1; 

% Invariant Time Data 
Cop_time.UTC = 1;
Cop_time.year = 2021; 
Cop_time.month = 6;
Cop_time.day = 21;
Cop_time.min = 0; 
Cop_time.sec = 0;

Cop_azimuth = zeros(24,1); 
Cop_zenith = Cop_azimuth;

%% Compute and Plot Results

for t = 1:24
    Bei_time.hour = t-1; 
    sun = sun_position(Bei_time, Bei_location); 
    Bei_azimuth(t) = sun.azimuth;  
    Bei_zenith(t) = sun.zenith; 
    
    Rey_time.hour = t-1; 
    sun = sun_position(Rey_time, Rey_location); 
    Rey_azimuth(t) = sun.azimuth;  
    Rey_zenith(t) = sun.zenith; 
    
    Qui_time.hour = t-1; 
    sun = sun_position(Qui_time, Qui_location); 
    Qui_azimuth(t) = sun.azimuth;  
    Qui_zenith(t) = sun.zenith; 
    
    Lon_time.hour = t-1; 
    sun = sun_position(Lon_time, Lon_location); 
    Lon_azimuth(t) = sun.azimuth;  
    Lon_zenith(t) = sun.zenith;     
    
    Cop_time.hour = t-1; 
    sun = sun_position(Cop_time, Cop_location); 
    Cop_azimuth(t) = sun.azimuth;  
    Cop_zenith(t) = sun.zenith;   
end

T = 0:23; 
figure(1);  
plot(T,Bei_azimuth);
grid on;
hold on; 
plot(T,Rey_azimuth);
plot(T,Qui_azimuth);
plot(T,Lon_azimuth);
plot(T,Cop_azimuth);
title('Diurnal Variation of Solar Azimuth Angle in Different Cities'); 
xlabel('Time of Day (h)'); 
ylabel('Azimuth Angle (degrees)'); 
legend('Beirut','Reykjavic','Quito','Longyearbyen','Copenhagen'); 

figure(2); 
plot(T,Bei_zenith); 
grid on; 
hold on; 
plot(T,Rey_zenith); 
plot(T,Qui_zenith);
plot(T,Lon_zenith);
plot(T,Cop_zenith);
title('Diurnal Variation of Solar Zenith Angle in Different Cities'); 
xlabel('Time of Day (h)'); 
ylabel('Zenith Angle (degrees)'); 
legend('Beirut','Reykjavic','Quito','Longyearbyen','Copenhagen'); 

figure
plot(Bei_azimuth,Bei_zenith);
grid on;
xlabel('Azimuth');ylabel('Zenith');
title ('Azimuth Vs Zenith for Beirut');
figure
plot(Qui_azimuth,Qui_zenith)
grid on;
xlabel('Azimuth');ylabel('Zenith');
title ('Azimuth Vs Zenith for Qui');
figure(3)
plot(T,Qui_azimuth)
hold on;
plot(T,Qui_zenith)
grid on;
legend('Azimuth','Zenith')

figure(4)
plot(T,Bei_azimuth)
hold on;
plot(T,Bei_zenith)
grid on;
legend('Azimuth','Zenith')
title('beirut')

