%% Variation of Solar Angles w.r.t Latitude 
location.longitude = 35.495480; 
location.altitude = 19; 

time.UTC = 2;
time.year = 2021; 
time.month = 6;
time.day = 21;
time.min = 0; 
time.sec = 0;

azimuth = zeros(24,1); 
zenith = azimuth; 
data.latitude = 0:10:80;
data.azimuth = zeros(24,9);
data.zenith = zeros(24,9);

for l = 1:9
    location.latitude = (l-1)*10; 
    for t = 1:24
        time.hour = t-1; 
        sun = sun_position(time, location); 
        azimuth(t) = sun.azimuth;  
        zenith(t) = sun.zenith;
    end
    data.azimuth(:,l) = azimuth; 
    data.zenith(:,l) = zenith; 
end

T = 0:23;
figure; 
for l = 1:9 
    plot(T,data.azimuth(:,l)); 
    hold on; 
end
title('Diurnal Variation of Azimuth Angle for Different Latitudes'); 
legend('10','20','30','40','50','60','70','80'); 

figure; 
for l = 1:9 
    plot(T,data.zenith(:,l)); 
    hold on; 
end
title('Diurnal Variation of Zenith Angle for Different Latitudes'); 
legend('10','20','30','40','50','60','70','80'); 

