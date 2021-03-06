function [ parameters, states ] = bci_Construct

% Filter construction demo
% 
% Perform any initialization; request BCI2000 parameters and states
% by returning parameter and state definition lines as demonstrated
% below.

% BCI2000 filter interface for Matlab
% juergen.mellinger@uni-tuebingen.de, 2005
% (C) 2000-2008, BCI2000 Project
% http://www.bci2000.org

parameters = { ...
  [ 'MatlabDemo list MyFilterOffsets= 3 0 0 0 0 0 0 ' ...
    '// A list of offsets added to the signal' ] ...
  [ 'MatlabDemo matrix MyFilterMatrix= 3 2 1 2 3 4 5 6 0 0 0 ' ...
    '// A demo filter matrix' ] ...
  [ 'MatlabDemo int MyRunCount= 1 0 0 0 ' ...
    '// A parameter modified by Matlab' ] ...
  [ 'MatlabDemo int MyReportBogusError= 1 % % % ' ...
    '// Report a bogus error (boolean)' ] ...
};
states = { ...
  'MyDemoState 4 0 0 0' ...
};
