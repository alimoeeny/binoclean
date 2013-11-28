function err = ali_ExpGen(varargin)
% ali_ExptGen generates a set of stim files to be user with a manual experiment 
% 
% Usage
% ali_ExpGen('experimentname', 'somename', 'basedir','usually /local/manstims', 
%     'numberofrepetitions', 8, 
%     'mainexpt', 'a struct that defines the main experiment with fields like st et ei nt', 
%     'oddball', 'a stuct that defins the oddball trials',
%     
%   )
%
% Example
% ali_ExpGen('basedir', '/local/AliManStims', 'experimentname', 'ali_CylinderTask', 'repetitions', 8, 'mainexpt', struct('et', 'dx', 'st', 'rds', 'nt', 8, 'em', 0, 'ie', 0.02, 'ce', 1, 'sa',3), 'oddball', struct('ce',0,'st', 'rds', 'op','-sq', 'sa', 0, 'sl', 0))  
%
% Depedencies
% parsevarargin.m
% ali_savestruct2file
%
% first version: 22/11/2013
% this version: 22/11/2013


arguments = parsevarargin(varargin);

if arguments.isKey('experimentname')
   experimentname = arguments('experimentname');
else
    disp('ERROR, experimentname is a required argument');
    return
end

basedir = '/local/AliManStims/';
if arguments.isKey('basedir')
    basedir = arguments('basedir');
    if basedir(end) ~= '/'
        basedir = [basedir '/'];
    end
end

numberofrepetitions = 4;
if arguments.isKey('numberofrepetitions')
    numberofrepetitions =arguments('numberofrepetitions');
end

if arguments.isKey('mainexpt')
    mainexpt = arguments('mainexpt');
end

%st 
if isfield(mainexpt, 'st')
    stimulustype = mainexpt.st;
else
    disp('main experiment needs st');
    return
end
%et
if isfield(mainexpt, 'et')
    experimentalvariable = mainexpt.et;
else
    disp('"et" is required');
    return
end
%nt
if isfield(mainexpt, 'nt')
    numberofregularconditions = mainexpt.nt;
else
    disp('main experiemtn needs nt');
    return
end
%mt
if isfield(mainexpt,'em')
    meanstimvalue = mainexpt.em;
else
    disp('main experiemtn needs em');
    return
end

%in
if isfield(mainexpt,'ie')
    increment = mainexpt.ie;
else
    disp('main experiemtn needs ie');
    return
end

%op
if isfield(mainexpt, 'op')
    mainop = mainexpt.op;
end


if arguments.isKey('oddballexpt')
    oddballexpt = arguments('oddballexpt');
end


% remove the pervious stim files and directory
if exist([basedir experimentname],'dir')
    delete([basedir experimentname '/*']);
    rmdir([basedir experimentname]);
end
[SUCCESS,~,~] = mkdir([basedir experimentname]);
if SUCCESS == 0
    disp('ERROR CREATING THE stim DIRECTORY, CANNOT CONTINUE');
    err = 'ERROR CREATING THE stim DIRECTORY';
    return
end


stimcounter = 0; % this is the suffix of the stim files and is zero based

stimvalues = -floor(numberofregularconditions / 2) * increment + meanstimvalue :increment: floor(numberofregularconditions / 2) * increment + meanstimvalue;
stimvalues = stimvalues(1:numberofregularconditions);
for i = stimvalues
    clear stim
    stim.(experimentalvariable) = i;
    stim.psyv = i;
    stim.st = stimulustype;
    
    if isfield(mainexpt, 'ce') 
        stim.ce = mainexpt.ce;
    elseif isfield(oddballexpt, 'ce')
        stim.ce = abs(oddballexpt.ce - 1);
    end
    if isfield(mainexpt, 'sl') 
        stim.sl = mainexpt.sl;
    elseif isfield(oddballexpt, 'sl')
        stim.sl = abs(oddballexpt.sl - 1);
    end
    if isfield(mainexpt, 'op')
        stim.op = mainexpt.op;
    elseif isfield(oddballexpt, 'op')
        if strfind(oddballexpt.op, '-sq')
            stim.op = '+sq';
        elseif strfind(oddballexpt.op, '+sq')
            stim.op = '-sq';
        end
    end
    
    
    % sa should be last or near the end
    if isfield(mainexpt, 'sa') 
        stim.sa = mainexpt.sa;
    elseif isfield(oddballexpt, 'sa')
        stim.sa = abs(oddballexpt.sa - 1);
    end
    
    
    
    filename = [basedir experimentname '/stim' num2str(stimcounter)];
    stimcounter = stimcounter + 1;
    %err = ali_savemap2file(stim, filename);
    err = ali_savestruct2file(stim, filename);
    if err ~= ''
        disp('SOMETHING WENT WRONG HERE SAVEING STIM FILE')
        return
    end
end


% oddbasll expt

oddballstim = oddballexpt;

oddballstim.psyv = 0;

filename = [basedir experimentname '/stim' num2str(stimcounter)];
stimcounter = stimcounter + 1;
err = ali_savestruct2file(oddballstim, filename);
if err ~= ''
    disp('SOMETHING WENT WRONG HERE SAVEING BASELINE EXPERIMENT')
    return
end

% - - - - - - - - - - - - STIM ORDER file
stims = 0:stimcounter-1;
% to make the oddball apear twice as much as others
if arguments.isKey('oddballratio') 
	stims(end+1) = stims(end);
end

overallstimorder = [];
for rpt = 1:numberofrepetitions
    overallstimorder = [overallstimorder stims(randperm(length(stims)))];
end
filename = [basedir experimentname '/stimorder'];
dlmwrite(filename, overallstimorder, ' ');

disp(['Total stim files created:', num2str(stimcounter)]);
err = '';
