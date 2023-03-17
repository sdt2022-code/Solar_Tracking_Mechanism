function It = SolarIrradiance2(beta,gamma,phi,omega,A,date)
%% Description 
% This function calculates the hourly-averaged solar irradiance incident on
% a tilted surface. 

% Input Parameters 
% beta: module zenith angle 
% gamma: module azimuth angle 
% delta: solar declination angle 
% omega: solar hour angle 
% A: altitude in km 
% date: calendar date vector of format: [year month day]

%% Parameters 
rho = 0.3; 

%% Setting Appropriate Range for Azimuth Angle (-180,180)
gamma = -(180-gamma); 

%% Day of Year 
date = datetime(date); 
n = day(date,'dayofyear');

%% Declination Angle 
delta = 23.45*sind(360*(284+n)/365); 

%% Angle of Incidence 
Cos_theta = sind(delta)*sind(phi)*cosd(beta) - sind(delta)*cosd(phi)*sind(beta)*cosd(gamma) + cosd(delta)*cosd(phi)*cosd(beta)*cosd(omega) + cosd(delta)*sind(beta)*sind(gamma)*sind(omega) + cosd(delta)*sind(phi)*sind(beta)*cosd(gamma)*cosd(omega);
beta_z = 0; 
Cos_theta_z = sind(delta)*sind(phi)*cosd(beta_z) - sind(delta)*cosd(phi)*sind(beta_z)*cosd(gamma) + cosd(delta)*cosd(phi)*cosd(beta_z)*cosd(omega) + cosd(delta)*sind(beta_z)*sind(gamma)*sind(omega) + cosd(delta)*sind(phi)*sind(beta_z)*cosd(gamma)*cosd(omega);
R = Cos_theta/Cos_theta_z; 

%% Transmittances 
a0 = 0.4237 - 0.00821*(6-A)^2;
a1 = 0.5055 - 0.00595*(6.5-A)^2; 
k = 0.2711 - 0.01858*(2.5-A)^2; 
Tb = a0 + a1*exp(-k/Cos_theta_z); 
Td = 0.271 - 0.294*Tb;

%% Irradiances 
Cs = 1367; 
Kt = (-8e-6)*n^2 + 0.003*n + 0.37; 
I0 = Cs*(Tb+Td)*(1+0.033*cosd(360*n/365)*abs(Cos_theta_z)); 
Ib = (Tb/(Tb+Td))*Kt*I0; 
Id = (Td/(Tb+Td))*Kt*I0; 
I = Ib+Id; 
It = Ib*R + Id*((1+cosd(beta))/2) + rho*I*((1-cosd(beta))/2);

end 