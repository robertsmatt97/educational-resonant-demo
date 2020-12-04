%% Resonant Simulation without Serial Transmission
% Matthew Roberts
% Fall 2020
% Master's Thesis: Educational Demos for Resonant Converter Phenomena
% Displays Hypothetical Data with Simulation from LTSPICE data. 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%% Configure Settings Here %%%%%%%%%%
sim_filename = 'Sim Text Files\Simulation_Results_Full-Bridge.txt';
                                % Set the file path of the txt file
                                % exported from the LTSpice Simulation.
                                % Include the extension, '.txt' in the 
                                % name.

C_switch = 1;                   % Set to 1 if Cr = 227n (SW4 is on)
HB_switch = 0;                  % Set to 1 for Half-Bridge
                                % Set to 0 for Full-Bridge
                                % Reflect simulation to above 
                                
R_L_vals = [10,20,30,40,50];    % Vary Load values
Sim_R_L_val = 40;               % Define Load R from Simulation
Gain_Axis_Lim = 6;              % Define Plot Y limit of Gain Curves

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Misc Commands

close all
clc
format shortG
warning('off')
Plot_Setup = [C_switch, not(HB_switch), Sim_R_L_val, Gain_Axis_Lim];

%% Variables

V_in_min = 3.3;
V_in_max = 3.3;
n = 1;
M_min = 0.9 / n;
M_max = 4.75 / n;
V_out_min = M_min * V_in_max / n;
V_out_max = M_max * V_in_min / n;

R_L_min = 10;
R_L_max = 50;
R_eq_min = 8 *(n^2) * R_L_min / pi^2;
R_eq_max = 8 *(n^2) * R_L_max / pi^2;
C_1 = 0.00000004700;
C_2 = 0.0000002110;
L_1 = 0.0000120000;

L_n_vals = [0.67 1.00 2.00 2.4];
L_n_vals = transpose(L_n_vals);
C_r_vals = [C_1, C_2];
L_r_vals = L_1;
L_m = L_n_vals .* L_1;
L_n = 1.00;
C_x_L = [C_2 * L_1, C_1 * L_1];
f_r = 1 ./ (2 .* pi .* sqrt(C_x_L));
w_s = 2 * pi * f_r;

C_1_char = char(vpa(C_1*1000000000));
C_2_char = char(vpa(C_2*1000000000));
L_1_char = char(vpa(L_1*1000000));

disp(strcat("The first resonant C value is: ", C_1_char(1:4), " nF"))
disp(strcat("The second resonant C value is: ", C_2_char(1:5), " nF"))
disp(strcat("The resonant L value is: ", L_1_char(1:4), " uH"))

%% Ratings and Calculations

I_0 = V_out_max / R_L_max;
I_pri_rms = (pi * I_0) / (2 * n * sqrt(2)); 
I_sec_rms = I_pri_rms * n;
I_mag_rms = 0.901 * (V_out_max / (min(w_s) * min(L_m,[],'all')));
I_r_rms = sqrt((I_pri_rms^2) + (I_mag_rms^2));
I_sw_max = (sqrt(2) / 2) * I_sec_rms; 
I_diode_rat = (sqrt(2) / pi) * I_sec_rms;

I_pri_rms_char = char(vpa(I_pri_rms));
I_mag_rms_char = char(vpa(I_mag_rms));
I_r_rms_char = char(vpa(I_r_rms));
I_sw_max_char = char(vpa(I_sw_max));
I_diode_rat_char = char(vpa(I_diode_rat));

disp(strcat("The maximum RMS load current coming from the primary is: " ...
    , I_pri_rms_char(1:6), " A"))
disp(strcat("The maximum RMS Magnetizing current is: ", ...
    I_mag_rms_char(1:6), " A"))
disp(strcat("The maximum RMS Resonant current is: ", I_r_rms_char(1:6), ...
    " A"))
disp(strcat("The maximum windings current is: ", I_sw_max_char(1:6), " A"))
disp(strcat("The diode current rating is: ", I_diode_rat_char(1:3), " A"))

%% Reading Simulation Data

sim_data = readmatrix(sim_filename, 'Range', 2, ... 
    'OutputType', 'double','Delimiter','	');

%% Gain v.s. Normalized Frequency Plot Property Options

fig = figure('units',  'normalized', 'outerposition', [0 0 1 1]);

sgtitle("Gain v.s. Normalized Frequency")

b = uicontrol('Parent', fig, 'Style', 'slider', 'units', 'normalized', ... 
    'Position', [0.36,0.12,0.3,0.04], 'value', L_n, 'min', 0.55, 'max', ...
    2.4);
bgcolor = fig.Color;
bl1 = uicontrol('Parent', fig, 'Style', 'text', 'units', 'normalized', ... 
    'Position', [0.34,0.08,0.04,0.03], 'String', '0.67', ...
    'BackgroundColor', bgcolor);
bl2_1 = uicontrol('Parent', fig, 'Style', 'text', 'units', ... 
    'normalized', 'Position', [0.445,0.08,0.04,0.03], 'String', '1.00', ...
    'BackgroundColor', bgcolor);
bl2_2 = uicontrol('Parent', fig, 'Style', 'text', 'units', ... 
    'normalized', 'Position', [0.545,0.08,0.04,0.03], 'String', '2.00', ...
    'BackgroundColor', bgcolor);
bl2_3 = uicontrol('Parent', fig, 'Style', 'text', 'units', ...
    'normalized', 'Position', [0.64,0.08,0.04,0.03], 'String','4.5', ...
    'BackgroundColor', bgcolor);

% Callback Function when changing L_n slider
b.Callback = @(es,ed) plot_Gain(R_L_vals, C_r_vals, L_r_vals, L_n_vals, ...
    es.Value, 1, fig, f_r, sim_data, Plot_Setup); 

% Function to plot Curve
L_n = plot_Gain(R_L_vals, C_r_vals, L_r_vals, L_n_vals, L_n, 0, fig, ...
    f_r, sim_data, Plot_Setup);

   

%% Gain v.s. Switching Frequency Plot Property Options

fig2 = figure('units', 'normalized', 'outerposition', [0 0 1 1]);
sgtitle("Gain v.s. Switching Frequency")

b = uicontrol('Parent', fig2, 'Style', 'slider', 'units', 'normalized', ...
    'Position', [0.36,0.12,0.3,0.04], 'value', L_n, 'min', 0.55, 'max', ...
    2.4);
bgcolor = fig2.Color;
bl1 = uicontrol('Parent', fig2, 'Style', 'text', 'units', 'normalized', ...
    'Position', [0.34,0.08,0.04,0.03], 'String', '0.67', ...
    'BackgroundColor', bgcolor);
bl2_1 = uicontrol('Parent', fig2, 'Style', 'text', 'units', ...
    'normalized', 'Position', [0.445,0.08,0.04,0.03], 'String', '1.00', ...
    'BackgroundColor', bgcolor);
bl2_2 = uicontrol('Parent', fig2, 'Style', 'text', 'units', ...
    'normalized', 'Position', [0.545,0.08,0.04,0.03], 'String', '2.00', ...
    'BackgroundColor', bgcolor);
bl2_3 = uicontrol('Parent',fig2,'Style','text','units', 'normalized', ...
    'Position',[0.64,0.08,0.04,0.03], 'String','4.5','BackgroundColor', ...
    bgcolor);

% Callback Function when changing L_n slider
b.Callback = @(es,ed) plot_Gain_2(R_L_vals, C_r_vals, L_r_vals, ... 
    L_n_vals, es.Value, 1, fig2, f_r, sim_data, Plot_Setup); 

% Function to plot Curve
L_n = plot_Gain_2(R_L_vals, C_r_vals, L_r_vals, L_n_vals, L_n, 0, fig2, ...
    f_r, sim_data, Plot_Setup);  


%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Functions
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Gain v.s. Normalized Frequency Function
function L_n = plot_Gain(R_L_vals, C_r_vals, L_r_vals, L_n_vals, L_n, ...
    overwrite, fig, f_r, sim_data, Plot_Setup)
   
    % Modifying Parameters
    L_div_C = [L_r_vals / C_r_vals(2), L_r_vals / C_r_vals(1)];
    new_C_r = repmat([C_r_vals(2), C_r_vals(1)], 1, 2);
    new_L_r = repmat([L_r_vals(1), L_r_vals(1)], 1, 2);
    R_L_mat = ones([length(L_div_C), length(R_L_vals)]);
    R_L_mat = R_L_mat(:,1) .* R_L_vals;
    R_eq_mat = [(2 .* R_L_mat ./ pi^2);(8 .* R_L_mat ./ pi^2)];
    R_L_mat = repmat(R_L_mat, 2, 1);
    L_div_C = repmat(L_div_C, 1, 2);
    f_r = repmat(f_r, 1, 2);
    temp_mat = repmat(L_n_vals, [1 length(L_n)]);
    [~, closestIndex] = min(abs(temp_mat - L_n.'));
    L_n = L_n_vals(closestIndex);
    if L_n == 2.4
        L_n = 4.46;
    end
    
    % Plot Setup
    plots = ones([1, length(R_L_mat(1,:))]);
    syms f
    plot_row = floor(sqrt(length(R_L_mat(:,1))));
    plot_column = ceil(sqrt(length(R_L_mat(:,1))));
    if ((plot_row * plot_column) < length(R_L_mat(:,1)))
        plot_row = plot_row + 1;
    end
    
    % Plotting Loop
    for c = 1:length(R_L_mat(:,1))
        subplot(plot_row, plot_column, c)
        if (overwrite == 1)
            cla
        end
        for d = 1:length(R_L_mat(1,:))
            Q_s = (sqrt(L_div_C(c))) / R_eq_mat(c, d);
            if(c > length(R_eq_mat(:,1)) / 2)
                M_g_arr = (L_n * f * f) / ((((L_n + 1) * f * f) - 1) + ...
                    1i * ((f*Q_s) * L_n * (f * f - 1))) / 2;
            else
                M_g_arr = (L_n * f * f) / ((((L_n + 1) * f * f) - 1) + ...
                    1i * ((f * Q_s) * L_n * (f * f - 1)));
            end
            p = newplot;
            p.NextPlot = 'add';
            plots(d) = fplot(abs(M_g_arr));
        end
        
        % Title Setup
        plot_title_C = char(vpa(new_C_r(c) * 1000000000));
        plot_title_L = char(vpa(new_L_r(c) * 1000000));
        if(c > length(R_eq_mat(:,1)) / 2)
            title(strcat("Full-Bridge, C_{r} = ", plot_title_C, ...
                " nF, L_{r} = ", plot_title_L," uH"))
        else
            title(strcat("Half-Bridge, C_{r} = ", plot_title_C, ...
                " nF, L_{r} = ", plot_title_L," uH"))
        end
        
        % Setting up the Legend
        labels2 = strings(1, length(R_L_mat(c, :)));
        R_labels = char(arrayfun(@char, vpa(R_L_mat(c, :)), 'uniform', 0));
        for e = 1:length(R_L_mat(c, :))
            if R_L_mat(c, e) >= 100
            labels2(e) = strcat("R_{L} = ", R_labels(e, 1:3));
            else 
                labels2(e) = strcat("R_{L} = ", R_labels(e, 1:2));
            end
        end
        
        % Add Simulation Measurements to Plot
        if((round((c / length(R_eq_mat(:, 1))) - 0.000001) ... 
                == Plot_Setup(2)) && (mod(c, 2) == Plot_Setup(1)))
            plots(end+1) = plot(sim_data(:, 1) / (f_r(c)), ... 
                sim_data(:, 2), 'k--');
            Sim_label = char(arrayfun(@char, vpa(Plot_Setup(3)), ...
                'uniform', 0));
            labels2(end+1) = strcat("Sim, R_{L} = ", Sim_label(1:2));
        end
        
        % Plot Options
        legend(plots, labels2)
        xlim([0 2])
        ylim([0 Plot_Setup(4)])
        xlabel('f_{n}')
        ylabel('Gain')
        grid on
        splot_width = 0.85 / (plot_column);
        splot_height = 0.7 / (plot_row);
        splot_wpos = (mod(c - 1, plot_column)) * 0.5 + 0.025;
        splot_hpos = 0.56 - ((floor((c / plot_row) - 0.0000001) * 0.36));
        set(gca, 'units', 'normalized', 'outerposition', ... 
            [splot_wpos, splot_hpos, splot_width, splot_height])
    end
    
    % Slider Options
    bl3 = uicontrol('Parent', fig, 'Style', 'text', 'units', ... 
        'normalized', 'Position', [0.45,0.165,0.12,0.03], 'String', ...
        strcat("Current L_n value: ", char(vpa(L_n, 2))), ...
        'BackgroundColor', fig.Color);
end

%% Gain v.s. Switching Frequency Function
function L_n = plot_Gain_2(R_L_vals, C_r_vals, L_r_vals, L_n_vals, L_n, ...
    overwrite, fig2, f_r, sim_data, Plot_Setup)
    
    % Modifying parameters
    L_div_C = [L_r_vals / C_r_vals(2), L_r_vals / C_r_vals(1)];
    new_C_r = repmat([C_r_vals(2), C_r_vals(1)], 1, 2);
    new_L_r = repmat([L_r_vals(1), L_r_vals(1)], 1, 2);
    R_L_mat = ones([length(L_div_C), length(R_L_vals)]);
    R_L_mat = R_L_mat(:,1) .* R_L_vals;
    R_eq_mat = [(2 .* R_L_mat ./ pi^2);(8 .* R_L_mat ./ pi^2)];
    R_L_mat = repmat(R_L_mat, 2, 1);
    L_div_C = repmat(L_div_C, 1, 2);
    f_r = repmat(f_r, 1, 2);
    temp_mat = repmat(L_n_vals, [1 length(L_n)]);
    [~, closestIndex] = min(abs(temp_mat - L_n.'));
    L_n = L_n_vals(closestIndex);
    if L_n == 2.4
        L_n = 4.46;
    end
    
    % Plot Setup
    plots = ones([1, length(R_L_mat(1,:))]);
    syms f
    plot_row = floor(sqrt(length(R_L_mat(:,1))));
    plot_column = ceil(sqrt(length(R_L_mat(:,1))));
    if ((plot_row * plot_column) < length(R_L_mat(:,1)))
        plot_row = plot_row + 1;
    end
    
    % Plotting Loop
    for c = 1:length(R_L_mat(:,1))
        subplot(plot_row, plot_column, c)
        if (overwrite == 1)
            cla
        end
        for d = 1:length(R_L_mat(1,:))
            f_s = f ./ f_r(c);
            Q_s = (sqrt(L_div_C(c))) / R_eq_mat(c, d);
            if(c > length(R_eq_mat(:,1)) / 2)
                M_g_arr = ((L_n * f_s * f_s) / ((((L_n + 1) * f_s * ... 
                    f_s) - 1) + (1i * (f_s * Q_s) * L_n * ... 
                    (f_s * f_s - 1)))) / 2;
            else
                M_g_arr = ((L_n * f_s * f_s) / ((((L_n + 1) * f_s * ... 
                    f_s) - 1) + (1i * (f_s * Q_s) * L_n * ... 
                    (f_s * f_s - 1))));
            end
            p = newplot;
            p.NextPlot = 'add';
            plots(d) = fplot(abs(M_g_arr));
        end
        
        % Title Setup
        plot_title_C = char(vpa(new_C_r(c) * 1000000000));
        plot_title_L = char(vpa(new_L_r(c) * 1000000));
        if(c > length(R_eq_mat(:,1)) / 2)
            title(strcat("Full-Bridge, C_{r} = ", plot_title_C, ... 
                " nF, L_{r} = ", plot_title_L, " uH"))
        else
            title(strcat("Half-Bridge, C_{r} = ", plot_title_C, ... 
                " nF, L_{r} = ", plot_title_L, " uH"))
        end
        
        % Setting up the legend
        labels2 = strings(1, length(R_L_mat(c, :)));
        R_labels = char(arrayfun(@char, vpa(R_L_mat(c, :), 3), ... 
            'uniform', 0));
        for e = 1:length(R_L_mat(c, :))
            if R_L_mat(c, e) >= 100
            labels2(e) = strcat("R_{L} = ", R_labels(e, 1:3));
            else 
                labels2(e) = strcat("R_{L} = ", R_labels(e, 1:2));
            end
        end
        
        % Add Simulation Measurements to Plot
        if((round(((c / length(R_eq_mat(:,1)))) - 0.000001) ... 
                == Plot_Setup(2)) && (mod(c, 2) == Plot_Setup(1)))
            plots(end+1) = plot(sim_data(:,1), sim_data(:,2), 'k--');
            Sim_label = char(arrayfun(@char, vpa(Plot_Setup(3)), ...
                'uniform', 0));
            labels2(end+1) = strcat("Sim, R_{L} = ", Sim_label(1:2));
        end
        
        % Plot Options
        legend(plots, labels2)
        if (mod(c, plot_row) == 0)
            xlim([100000 300000])
        else
            xlim([0 200000])
        end
        ylim([0 Plot_Setup(4)])
        xlabel('f_{s}')
        ylabel('Gain')
        grid on
        splot_width = 0.85 / (plot_column);
        splot_height = 0.7 / (plot_row);
        splot_wpos = (mod(c - 1, plot_column)) * 0.5 + 0.025;
        splot_hpos = 0.56 - ((floor((c / plot_row) - 0.0000001) * 0.36));
        set(gca, 'units', 'normalized', 'outerposition', ...
            [splot_wpos, splot_hpos, splot_width, splot_height])
    end
    
    % Slider Options
    bl3 = uicontrol('Parent', fig2, 'Style', 'text', 'units', ... 
        'normalized', 'Position', [0.45,0.165,0.12,0.03], 'String', ...
        strcat("Current L_n value: ", char(vpa(L_n, 2))), ...
        'BackgroundColor', fig2.Color); %#ok<*NASGU>
end

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% End of Code
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%