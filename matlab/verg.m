function verg(varargin)
%binoc
%GUI for running binoclean via pipe.

if length(varargin) & ishandle(varargin{1})
    f = varargin{1};
    while ~isfigure(f)
        f = get(f,'parent');
    end
    DATA = get(f,'UserData');
    varargin = varargin(3:end);
else
TOPTAG = 'binoc';
it = findobj('Tag',TOPTAG,'type','figure');
if isempty(it)
    DATA.name = 'Binoc';
    DATA.tag.top = 'Binoc';
    DATA = SetDefaults(DATA);
%open pipes to binoc and read status from there before reading stimfile and sending
%changes to binoc
    DATA = OpenPipes(DATA);
    if length(varargin) && exist(varargin{1},'file')
        DATA = ReadStimFile(DATA,varargin{1});
    end
    DATA = InitInterface(DATA);
    DATA = SetExptMenus(DATA);
    SetGui(DATA);
    set(DATA.toplevel,'UserData',DATA);
end
end
if length(varargin)
    if strncmpi(varargin{1},'close',5)
        if isfield(DATA,'timerobj') & isvalid(DATA.timerobj)
            stop(DATA.timerobj);
        end
        f = fields(DATA.tag);
        for j = 1:length(f)
            CloseTag(DATA.tag.(f{j}));
        end
        return;
    elseif strncmpi(varargin{1},'quick',5)
        DATA = ReadStimFile(DATA, varargin{2});
    end
end

function DATA = InterpretLine(DATA, line)

strs = textscan(line,'%s','delimiter','\n');
for j = 1:length(strs{1})
    s = regexprep(strs{1}{j},'\s+\#.*$','');
    eid = strfind(s,'=');
    if length(s) == 0
    elseif s(1) == '#' %defines stim code/label
        [a,b] = sscanf(s,'#%d %s');
%        a = a(1);
%        id = find(s == ' ');
%        DATA.comcodes(a).code = s(id(1)+1:id(2)-1);
%        DATA.comcodes(a).label = s(id(2)+1:end);
%        DATA.comcodes(a).const = a;
    elseif strncmp(s,'electrdode',6)
        estr = s(eid(1)+1:end);
        DATA.electrodestrings = {DATA.electrodestrings{:} estr};
    elseif strncmp(s,'TOGGLE',6)
        id = strfind(s,' ');
        cc = s(id(1)+1:id(2)-1);
        if ~isfield(DATA.optionflags,cc)
            DATA.optionflags.(cc) = 0;
        end
        DATA.optionstrings.(cc) = s(id(2)+1:end);
    elseif strncmp(s,'STIMTYPE',6)
        id = strfind(s,' ');
        code = str2num(s(id(1)+1:id(2)-1))+1;
        DATA.stimulusnames{code} = s(id(2)+1:end);
    elseif strncmp(s,'CODE',4)
        id = strfind(s,' ');
        code = str2num(s(id(2)+1:id(3)-1))+1;
        DATA.comcodes(code).label = s(id(3)+1:end);
        DATA.comcodes(code).code = s(id(1)+1:id(2)-1);
        DATA.comcodes(code).const = code;
    elseif strncmp(s,'exps',4)
        ex = 1;
        DATA.expts{ex} = [];
        pid = strfind(s,'+');
        for j = 1:length(pid)
            if j == length(pid)
                x = s(pid(j)+1:end);
            else
                x = s(pid(j)+1:pid(j+1)-1);
            end
            if length(x) > 1
            eid = strmatch(x,{DATA.comcodes.code});
            DATA.expts{ex} = [DATA.expts{ex} eid];
            end
        end
        ex = 1;
            
    elseif strncmp(s,'EXPTSTART',8)
        DATA.inexpt = 1;
    elseif strncmp(s,'EXPTOVER',8)
        DATA.inexpt = 0;
    elseif strncmp(s,'Expts1',6)
        DATA.extypes{1} = sscanf(s(8:end),'%d');
        DATA.extypes{1} = DATA.extypes{1}+1;
        DATA = SetExptMenus(DATA);
    elseif strncmp(s,'Expts2',6)
        DATA.extypes{2} = sscanf(s(8:end),'%d');
        DATA.extypes{2} = DATA.extypes{2}+1;
        DATA = SetExptMenus(DATA);
    elseif strncmp(s,'Expts3',6)
        DATA.extypes{3} = sscanf(s(8:end),'%d');
        DATA.extypes{3} = DATA.extypes{3}+1;
        DATA = SetExptMenus(DATA);
    elseif strncmp(s,'winpos=',7)
        DATA.winpos{1} = sscanf(s(8:end),'%d');
    elseif strncmp(s,'optionwinpos=',10)
        DATA.winpos{2} = sscanf(s(eid(1)+1:end),'%d');
    elseif strncmp(s,'penlogwinpos=',10)
        DATA.winpos{3} = sscanf(s(eid(1)+1:end),'%d');

    elseif strncmp(s,'STIMC ',6)
        DATA.trialcounts = sscanf(s(7:end),'%d');
        ShowStatus(DATA);
    elseif strncmp(s,'qe=',3)
        id = strfind(s,'"');
        if length(id)
            s = s(id(end)+1:end);
        else
        s = s(4:end);
        end
        [a,b] = fileparts(s);
        n = length(DATA.quickexpts)+1;
        DATA.quickexpts(n).name = b;
        DATA.quickexpts(n).filename = s;
    elseif strncmp(s,'et',2)
        DATA.exptype{1} = sscanf(s,'et=%s');
    elseif strncmp(s,'e2',2)
        DATA.exptype{2} = sscanf(s,'e2=%s');
    elseif strncmp(s,'e3',2)
        DATA.exptype{3} = sscanf(s,'e3=%s');
    elseif strncmp(s,'nt',2)
        DATA.nstim(1) = sscanf(s,'nt=%d');
    elseif strncmp(s,'n2',2)
        DATA.nstim(2) = sscanf(s,'n2=%d');
    elseif strncmp(s,'n3',2)
        DATA.nstim(3) = sscanf(s,'n3=%d');
    elseif strncmp(s,'ei',2)
        DATA.incr(1) = sscanf(s,'ei=%f');
        DATA.binoc.ei = DATA.incr(1);
    elseif strncmp(s,'i2',2)
        DATA.incr(2) = sscanf(s,'i2=%f');
        DATA.binoc.i2 = DATA.incr(2);
    elseif strncmp(s,'i3',2)
        DATA.incr(3) = sscanf(s,'i3=%f');
        DATA.binoc.i3 = DATA.incr(3);
    elseif strncmp(s,'em',2)
        DATA.mean(1) = sscanf(s,'em=%d');
        DATA.binoc.em = DATA.mean(1);
    elseif strncmp(s,'m2',2)
        DATA.mean(2) = sscanf(s,'m2=%d');
        DATA.binoc.m2 = DATA.mean(2);
    elseif strncmp(s,'m3',2)
        DATA.mean(3) = sscanf(s,'m3=%d');
        DATA.binoc.m3 = DATA.mean(3);
    elseif strncmp(s,'op',2)
        f = fields(DATA.optionflags);
        if strncmp(s,'op=0',4) %everything else off
            for j = 1:length(f) DATA.optionflags.(f{j}) = 0; end
        end
        s = [s '+'];
        id = regexp(s,'[+-]');
        
        for j= 1:length(id)-1
            code = strmatch(s(id(j)+1:id(j+1)-1),f);
            if isempty(code)
                fprintf('No Code for %s\n,',s(id(j):end));
            elseif s(id(j)) == '+'
            DATA.optionflags.(f{code}) = 1;
            else
            DATA.optionflags.(f{code}) = 0;
            end
        end
    elseif strncmp(s, 'stepperxy', 8)
    elseif strncmp(s, 'penwinxy', 8)
    elseif strncmp(s, 'optionwinxy', 8)
    elseif strncmp(s, 'psychfile', 8)

    elseif strncmp(s, 'st', 2)
        id = strmatch(s(4:end),DATA.stimulusnames,'exact');
        DATA.stimtype(1) = id;
    elseif strncmp(s, 'Bs', 2)
             DATA.stimtype(2) = strmatch(s(4:end),DATA.stimulusnames,'exact');
    elseif s(1) == 'E'
        if strncmp(s,'EBCLEAR',5)
            DATA.exptstimlist{2} = {};
        elseif s(2) == 'B'
            n = sscanf(s(3:end),'%d');
            id = findstr(s,' ');
            if length(n)
                DATA.exptstimlist{2}{n(1)+1} = s(id(1)+1:end);
                if isfield(DATA,'toplevel')
                    it = findobj(DATA.toplevel,'Tag','Expt2StimList');
                    if length(it) == 1
                        set(it,'string',DATA.exptstimlist{2});
                    end
                end
            end
        else
            n = sscanf(s(2:end),'%d');
            id = findstr(s,' ');
            if length(n)
                DATA.exptstimlist{1}{n(1)+1} = s(id(1)+1:end);
                if isfield(DATA,'toplevel')
                    it = findobj(DATA.toplevel,'Tag','Expt1StimList');
                    if length(it) == 1
                        set(it,'string',DATA.exptstimlist{1});
                    end
                end
            end
            if strncmp(s,'ECLEAR',5)
                DATA.exptstimlist{1} = {};
            end
        end
    elseif strmatch(s,{DATA.comcodes.code})
        id = strmatch(s,{DATA.comcodes.code});
    else
        id = strfind(s,'=');
        if id
            code = s(1:id(1)-1);
            if isempty(strmatch(code, {'1t' '2t' '3t' '4t'})) %illegal names
            val = sscanf(s(id(1)+1:end),'%f');
            DATA.binoc.(code) = val;
            end
        end
    end
end

function DATA = ReadStimFile(DATA, name)
        
fid = fopen(name,'r');
if fid > 0
tline = fgets(fid);
while ischar(tline)
    DATA = InterpretLine(DATA,tline);
    if DATA.outid > 0
    fprintf(DATA.outid,tline);
    end
    tline = fgets(fid);
end
fclose(fid);
end

for ex = 1:3
    if length(DATA.expts{ex})
        id = find(~ismember(DATA.expmenuvals{ex}, DATA.expts{ex}));
        DATA.expmenuvals{ex} = [DATA.expts{ex} DATA.expmenuvals{ex}(id)];
    end
    DATA = SetExptMenus(DATA);
end


function DATA = OpenPipes(DATA)
        
DATA.outpipe = '/tmp/binocinputpipe';
DATA.inpipe = '/tmp/binocoutputpipe';

DATA.outid = fopen(DATA.outpipe,'w');
DATA.inid = fopen(DATA.inpipe,'r');
fprintf(DATA.outid,'NewMatlab\n');
DATA = ReadFromBinoc(DATA,'reset');
fprintf(DATA.outid,'QueryState\n');
DATA = ReadFromBinoc(DATA);
SetGui(DATA);
        
function DATA = SetDefaults(DATA)

scrsz = get(0,'Screensize');
DATA.optionflags.ts = 0;
DATA.showflags.ts = 1;
DATA.showflags.cf = 1;
DATA.showflags.wt = 1;
DATA.verbose = 1;
DATA.inexpt = 0;
DATA.datafile = [];
DATA.electrodestrings = {};
DATA.electrodestring = 'default';
DATA.monkey = 'lem';
DATA.penid = 0;
DATA.stimulusnames{1} = 'none';
DATA.stimulusnames{4} = 'grating';
DATA.stimulusnames{3} = 'rds';
DATA.stimulusnames{2} = 'gabor';
DATA.stimulusnames{5} = 'bar';
DATA.stimulusnames{6} = 'circle';
DATA.stimulusnames{7} = 'rectangle';
DATA.stimulusnames{8} = 'test';
DATA.stimulusnames{9} = 'square';
DATA.stimulusnames{10} = 'probe';
DATA.stimulusnames{11} = '2grating';
DATA.stimulusnames{12} = 'cylinder';
DATA.stimulusnames{13} = 'corrug';
DATA.stimulusnames{14} = 'sqcorrug';
DATA.stimulusnames{15} = 'twobar';
DATA.stimulusnames{16} = 'rls';


DATA.comcodes = [];
DATA.winpos{1} = [10 scrsz(4)-480 300 450];
DATA.winpos{2} = [10 scrsz(4)-480 300 450];
DATA.winpos{3} = [];
DATA.outid = 0;
DATA.incr = [0 0 0];
DATA.nstim = [0 0 0];
DATA.quickexpts = [];
DATA.stepsize = [20 10];
DATA.stepperpos = -2000;
DATA.tag.stepper = 'Stepper';
DATA.tag.options = 'Options';
DATA.tag.penlog = 'Penetration Log';
DATA.comcodes(1).label = 'Xoffset';
DATA.comcodes(1).code = 'xo';
DATA.comcodes(1).const = 1;
DATA.expts{1} = [];
DATA.expts{2} = [];
DATA.expts{3} = [];
DATA.expmenuvals{1} = [];
DATA.expmenuvals{2} = [];
DATA.expmenuvals{3} = [];
DATA.extypes{1} = [];
DATA.extypes{2} = [];
DATA.extypes{3} = [];
DATA.exptype{1} = 'e0';
DATA.exptype{2} = 'e0';
DATA.exptype{3} = 'e0';
DATA.stimtype(1) = 1;
DATA.stimtype(2) = 1;
DATA.mean = [0 0 0];
DATA.incr = [0 0 0];
DATA = ReadStimFile(DATA, '/local/verg.setup');

for j = 1:3 
DATA.expmenucodes{j} = {};
DATA.expstrs{j} = {};
DATA.expmenuvals{j} = [];
end
for j = 1:length(DATA.comcodes)
    if ismember(DATA.comcodes(j).const,DATA.extypes{1})
        DATA.expstrs{1} = {DATA.expstrs{1}{:} DATA.comcodes(j).label};
        DATA.expmenuvals{1} = [DATA.expmenuvals{1} DATA.comcodes(j).const];
        DATA.expmenucodes{1} = {DATA.expmenucodes{1}{:} DATA.comcodes(j).code};
    end
    if ismember(DATA.comcodes(j).const,DATA.extypes{2})
        DATA.expstrs{2} = {DATA.expstrs{2}{:} DATA.comcodes(j).label};
        DATA.expmenuvals{2} = [DATA.expmenuvals{2} DATA.comcodes(j).const];
    end
    if ismember(DATA.comcodes(j).const,DATA.extypes{3})
        DATA.expstrs{3} = {DATA.expstrs{3}{:} DATA.comcodes(j).label};
        DATA.expmenuvals{3} = [DATA.expmenuvals{3} DATA.comcodes(j).const];
    end
end


function DATA = SetExptMenus(DATA)
for j = 1:3 
end



for m = 1:3
    if length(DATA.expmenuvals{m}) > 0
        DATA.expstrs{m} = {DATA.comcodes(DATA.expmenuvals{m}).label};
        DATA.expmenucodes{m} = {DATA.comcodes(DATA.expmenuvals{m}).code};
    else
    DATA.expmenucodes{m} = {};
    DATA.expstrs{m} = {};
    DATA.expmenuvals{m} = [];
    for j = 1:length(DATA.extypes{m})
        id = find([DATA.comcodes.const] == DATA.extypes{m}(j));
        if length(id) == 1
            DATA.expstrs{m} = {DATA.expstrs{m}{:} DATA.comcodes(id).label};
            DATA.expmenuvals{m} = [DATA.expmenuvals{m} DATA.comcodes(id).const];
            DATA.expmenucodes{m} = {DATA.expmenucodes{m}{:} DATA.comcodes(id).code};
        end
    end
    end
end

   
if isfield(DATA,'toplevel') %GUI is up
it = findobj(DATA.toplevel,'Tag','Expt1List');
set(it,'string',DATA.expstrs{1});
it = findobj(DATA.toplevel,'Tag','Expt2List');
set(it,'string',DATA.expstrs{2});
it = findobj(DATA.toplevel,'Tag','Expt3List');
set(it,'string',DATA.expstrs{3});
end

function ShowStatus(DATA)

s = sprintf('Trials %d/%d Ex:%d/%d',DATA.trialcounts(1),DATA.trialcounts(2),DATA.trialcounts(3),DATA.trialcounts(4));
if isfield(DATA,'toplevel')
set(DATA.toplevel,'Name',s);
end
fprintf('%s\n',s);

function DATA = InitInterface(DATA)

    scrsz = get(0,'Screensize');
    cntrl_box = figure('Position', DATA.winpos{1},...
        'NumberTitle', 'off', 'Tag',DATA.tag.top,'Name',DATA.name,'menubar','none');
    nr = 20;
    DATA.toplevel = cntrl_box;
    lst = uicontrol(gcf, 'Style','edit','String', '',...
        'HorizontalAlignment','left',...
        'Callback', {@TextEntered}, 'Tag','NextButton',...
'units','norm', 'Position',[0.01 0 0.98 1./nr]);
    DATA.txtui = lst;
     lst = uicontrol(gcf, 'Style','list','String', 'Commmand History',...
        'HorizontalAlignment','left',...
        'Max',10,'Min',0,...
        'Callback', {@TextEntered}, 'Tag','NextButton',...
'units','norm', 'Position',[0.01 1.01./nr 0.98 7/nr]);
   DATA.txtrec = lst;
    nc = 4;

    bp(1) = 0.01;
    bp(2) = 8./nr;
    bp(3) = 0.1;
    bp(4) = 1./nr;
    uicontrol(gcf,'style','text','string','File',  'units', 'norm', 'position',bp);
    
    bp(1) = bp(1)+bp(3);
    bp(3) = 1-bp(1);
    uicontrol(gcf,'style','edit','string',DATA.datafile, ...
        'units', 'norm', 'position',bp,'value',1,'Tag','DataFileName','callback',{@TextGui, 'uf'});
    
    
    bp(2) = 11./nr;
    bp(1) = 0.01;
    bp(4) = 1.3/nr;
    bp(3) = 0.1;
        uicontrol(gcf,'style','pushbutton','string','Stop', ...
        'Callback', {@RunButton, 2}, 'Tag','StopButton',...
        'units', 'norm', 'position',bp,'value',1);

    bp(2) = 12./nr;
    bp(1) = 0.01;
    bp(4) = 1.3/nr;
    bp(3) = 0.1;
    uicontrol(gcf,'style','edit','string',num2str(DATA.binoc.nr), 'Tag', 'binoc.nr', 'units', 'norm',...
        'callback',{@TextGui, 'nr'},'position',bp);
    
    bp(3) = 0.3;
    bp(2) = 9./nr;
    bp(4) = 4/nr;
    bp(1) = 0.12;
    uicontrol(gcf,'style','list','string',DATA.exptstimlist{1}, ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt1StimList','buttondownfcn',@EditList,'keypressfcn',@EditList);
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','list','string',num2str(DATA.nstim(2)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt2StimList');
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','list','string',num2str(DATA.nstim(3)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt3StimList');
    
    
    bp(1) = 0.01;
    bp(2) = bp(2)+bp(4);
    bp(3) = 0.1;
    bp(4) = 1.2/nr;
    uicontrol(gcf,'style','text','string','N stim',  'units', 'norm', 'position',bp);
    bp(1) = bp(1)+bp(3)+0.01;
    bp(3) = 0.3;
    uicontrol(gcf,'style','edit','string',num2str(DATA.nstim(1)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt1Nstim','callback',{@TextGui, 'nt'});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string',num2str(DATA.nstim(2)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt2Nstim','callback',{@TextGui, 'n2'});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string',num2str(DATA.nstim(3)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt3Nstim','callback',{@TextGui, 'n3'});

    bp(1) = 0.01;
    bp(2) = bp(2)+bp(4);
    bp(3) = 0.1;
    uicontrol(gcf,'style','text','string','Incr',  'units', 'norm', 'position',bp);
    bp(1) = bp(1)+bp(3)+0.01;
    bp(3) = 0.3;
    uicontrol(gcf,'style','edit','string',num2str(DATA.incr(1)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt1Incr','callback',{@TextGui, 'ei'});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string',num2str(DATA.incr(2)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt2Incr','callback',{@TextGui, 'i2'});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string',num2str(DATA.incr(3)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt3Incr','callback',{@TextGui, 'i3'});

    bp(1) = 0.01;
    bp(2) = bp(2)+bp(4);
    bp(3) = 0.1;
    uicontrol(gcf,'style','text','string','Mean',  'units', 'norm', 'position',bp);
    bp(1) = bp(1)+bp(3)+0.01;
    bp(3) = 0.3;
    uicontrol(gcf,'style','edit','string',num2str(DATA.mean(1)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt1Mean','callback',{@TextGui, 'em'});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string',num2str(DATA.mean(2)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt2Mean','callback',{@TextGui, 'm2'});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string',num2str(DATA.mean(3)), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt3Mean','callback',{@TextGui, 'm3'});

    
    bp(1) = 0.01;
    bp(2) = bp(2)+bp(4);
    bp(3) = 0.1;
    
    uicontrol(gcf,'style','pushbutton','string','Run', ...
        'Callback', {@RunButton, 1}, 'Tag','RunButton',...
        'units', 'norm', 'position',bp,'value',1);
    bp(1) = bp(1)+bp(3)+0.01;
    bp(3) = 0.3;
    uicontrol(gcf,'style','pop','string',DATA.expstrs{1}, ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt1List','callback',{@SetExpt, 1});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','pop','string',DATA.expstrs{2}, ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt2List','callback',{@SetExpt, 2});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','pop','string',DATA.expstrs{3}, ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Expt3List','callback',{@SetExpt, 3});
    
    bp(1) = 0.01;
    bp(2) = bp(2)+bp(4);
    bp(3) = 0.1;
    uicontrol(gcf,'style','text','string','Fore',  'units', 'norm', 'position',bp);
    bp(1) = bp(1)+bp(3)+0.01;
    bp(3) = 0.3;
    uicontrol(gcf,'style','pop','string',DATA.stimulusnames, ...
        'units', 'norm', 'position',bp,'value',DATA.stimtype(1),'Tag','ForeGroundType','callback',{@SetExpt, 4});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','pop','string',DATA.stimulusnames, ...
        'units', 'norm', 'position',bp,'value',DATA.stimtype(2),'Tag','BackGroundType','callback',{@SetExpt, 5});

    
    bp(1) = 0.01;
    bp(2) = 0.99-1/nr;
    bp(3) = 1/nc;
    bp(4) = 1./nr;
    bp(3) = 1./5;
    uicontrol(gcf,'style','checkbox','string','go', ...
        'units', 'norm', 'position',bp,'value',1,'Tag','go','callback',@GoToggle);
    f = fields(DATA.showflags);
    allf = fields(DATA.optionflags);
    for j = 1:length(f)
        id = strmatch(f{j},allf);
        if length(id) == 1
            str = DATA.optionstrings.(allf{id});
        else
            str = num2str(j);
        end
        bp(1) = bp(1)+bp(3);
        uicontrol(gcf,'style','checkbox','string',str, ...
            'units', 'norm', 'position',bp,'value',DATA.optionflags.(f{j}),'Tag',f{j},'callback',{@HitToggle, f{j}});
        if bp(1)+bp(3)*2 > 1
            bp(1) = 0.01;
            bp(2) = bp(2) - 1./nr;
        end

    end
    bp(3) = 1/nc;
    
    hm = uimenu(cntrl_box,'Label','File','Tag','BinocFileMenu');
    uimenu(hm,'Label','Close','Callback',{@verg, 'close'});
    hm = uimenu(cntrl_box,'Label','Quick','Tag','QuickMenu');
    for j = 1:length(DATA.quickexpts)
    uimenu(hm,'Label',DATA.quickexpts(j).name,'Callback',{@verg, 'quick', DATA.quickexpts(j).filename});
    end
    hm = uimenu(cntrl_box,'Label','Pop','Tag','QuickMenu');
    uimenu(hm,'Label','Stepper','Callback',{@StepperPopup});
    uimenu(hm,'Label','Penetration Log','Callback',{@PenLogPopup});
    uimenu(hm,'Label','Options','Callback',{@OptionPopup});
    uimenu(hm,'Label','Test','Callback',{@TestIO});
    uimenu(hm,'Label','Read','Callback',{@ReadIO, 1});
    uimenu(hm,'Label','GetState','Callback',{@ReadIO, 2});
    uimenu(hm,'Label','NewStart','Callback',{@ReadIO, 3});
    uimenu(hm,'Label','Stop Timer','Callback',{@ReadIO, 4});
    uimenu(hm,'Label','Start Timer','Callback',{@ReadIO, 5});
    DATA.timerobj = timer('timerfcn',{@CheckInput, DATA.toplevel},'period',2,'executionmode','fixedspacing');
    
    set(DATA.toplevel,'UserData',DATA);
    start(DATA.timerobj);

 function SetExpt(a,b, type)
     DATA = GetDataFromFig(a);
     val = get(a,'value');
    if type == 1
        fprintf(DATA.outid,'et=%s\n',DATA.comcodes(DATA.expmenuvals{1}(val)).code);
    elseif type == 2
        fprintf(DATA.outid,'e2=%s\n',DATA.comcodes(DATA.expmenuvals{2}(val)).code);
    elseif type == 3
        fprintf(DATA.outid,'e2=%s\n',DATA.comcodes(DATA.expmenuvals{3}(val)).code);
    elseif type == 4
        fprintf(DATA.outid,'st=%s\n',DATA.stimulusnames{val});
    elseif type == 5
        fprintf(DATA.outid,'bs=%s\n',DATA.stimulusnames{val});
    end
     
    
 function EditList(a,b)
     id = get(a,'value');
     str = get(a,'string');
     c = get(a,'UserData');
     sendvals = 0;
     
     DATA = GetDataFromFig(get(a,'parent'));
     
     if isempty(c) || c(1) == 0;
         c= 1;
         str{id} = ' ';
     else
         c(1) = c(1)+1;
         if length(c) > 1
             set(a,'value',c(2));
             id = c(2);
         end
     end
     if double(b.Character) == 13
         c(1) = 0;
         set(a,'UserData',[c(1)]);
         sendvals = 1;
     elseif strcmp(b.Key,'downarrow')
         c(1) = 0;
         set(a,'UserData',[c(1) (id+1)]);
         sendvals = 1;
     elseif strcmp(b.Key,'uparrow')
         c(1) = 0;
         set(a,'UserData',[c(1) (id-1)]);
         sendvals = 1;
     elseif length(b.Character)
             str{id}(c(1)) = b.Character;
             set(a,'string',str);
             set(a,'UserData',[c(1) id]);
     end

if sendvals
    tag = get(a,'Tag');
    if strcmp(tag,'Expt2Stimlist')
        c = 'EB';
    elseif strcmp(tag,'Expt3Stimlist')
        c = 'EC';
    else
        c = 'E';
    end

    for j = 1:length(str)
            fprintf(DATA.outid,'%s%d=%s\n',c,j-1,str{j});
    end
    fprintf(DATA.outid,'EDONE\n');
end

     fprintf('%d:%s\n',id,str{id});
     
function MenuGui(a,b)
     DATA = GetDataFromFig(a);
     strs = get(a,'string');
     val = get(a,'value');
     str = strs{val};
     tag = get(a,'Tag');
     switch tag
         case 'ElectrodeType'
             DATA.electrodestring = str;
     end
     set(DATA.toplevel,'UserData',DATA);
     
     
 function TextGui(a,b, type)
     DATA = GetDataFromFig(a);
     str = get(a,'string');
     switch type
         case 'nt'
             DATA.nstim(1) = str2num(str);
             fprintf(DATA.outid,'nt=%d\n',DATA.nstim(1));
             ReadFromBinoc(DATA);
         case 'n2'
             DATA.nstim(2) = str2num(str);
             fprintf(DATA.outid,'n2=%d\n',DATA.nstim(2));
             ReadFromBinoc(DATA);
         case 'n3'
             DATA.nstim(3) = str2num(str);
             fprintf(DATA.outid,'n3=%d\n',DATA.nstim(3));
             ReadFromBinoc(DATA);
         case 'em'
             DATA.mean(1) = str2num(str);
             fprintf(DATA.outid,'em=%.8f\n',DATA.mean(1));
             ReadFromBinoc(DATA);
         case 'm2'
             DATA.mean(2) = str2num(str);
             fprintf(DATA.outid,'m2=%.8f\n',DATA.mean(2));
             ReadFromBinoc(DATA);
         case 'm3'
             DATA.mean(3) = str2num(str);
             fprintf(DATA.outid,'m3=%.8f\n',DATA.mean(3));
             ReadFromBinoc(DATA);
         case 'ei'
             DATA.incr(1) = str2num(str);
             fprintf(DATA.outid,'ei=%.8f\n',DATA.incr(1));
             ReadFromBinoc(DATA);
         case 'i2'
             DATA.incr(2) = str2num(str);
             fprintf(DATA.outid,'i2=%.8f\n',DATA.incr(2));
             ReadFromBinoc(DATA);
         case 'i3'
             DATA.incr(3) = str2num(str);
             fprintf(DATA.outid,'i3=%.8f\n',DATA.incr(3));
             ReadFromBinoc(DATA);
         case 'st'
             DATA.stimtype(1) = strmatch(str,DATA.stimulusnames);
         case 'bs'
             DATA.stimtype(2) = strmatch(str,DATA.stimulusnames);
         otherwise
             DATA.binoc.(type) = str2num(str);
             fprintf(DATA.outid,'%s=%s\n',type,str);
             ReadFromBinoc(DATA);
             
             
     end
             
        
 function ReadIO(a,b, flag)
     DATA = GetDataFromFig(a);

     
     if flag == 2
         fprintf(DATA.outid,'QueryState\n');
        DATA = ReadFromBinoc(DATA);   
        SetGui(DATA);
     elseif flag == 3
         stop(DATA.timerobj)
         fprintf(DATA.outid,'NewMatlab\n');
        DATA = ReadFromBinoc(DATA,'reset');   
        SetGui(DATA);
        start(DATA.timerobj);
     elseif flag == 4
         stop(DATA.timerobj)
     elseif flag == 5
        start(DATA.timerobj);
     else
        DATA = ReadFromBinoc(DATA);   
        SetGui(DATA);
     end

 function SetGui(DATA)
     if ~isfield(DATA,'toplevel')
         return;
     end
    SetTextItem(DATA.toplevel,'Expt1Nstim',DATA.nstim(1));
    SetTextItem(DATA.toplevel,'Expt2Nstim',DATA.nstim(2));
    SetTextItem(DATA.toplevel,'Expt3Nstim',DATA.nstim(3));
    SetTextItem(DATA.toplevel,'Expt1Incr',DATA.incr(1));
    SetTextItem(DATA.toplevel,'Expt2Incr',DATA.incr(2));
    SetTextItem(DATA.toplevel,'Expt3Incr',DATA.incr(3));
    SetTextItem(DATA.toplevel,'Expt1Mean',DATA.mean(1));
    SetTextItem(DATA.toplevel,'Expt2Mean',DATA.mean(2));
    SetTextItem(DATA.toplevel,'Expt3Mean',DATA.mean(3));
    id = strmatch(DATA.exptype{1},DATA.expmenucodes{1});
    SetMenuItem(DATA.toplevel, 'Expt1List', id);
    id = strmatch(DATA.exptype{2},DATA.expmenucodes{2});
    SetMenuItem(DATA.toplevel, 'Expt2List', id);
    id = strmatch(DATA.exptype{3},DATA.expmenucodes{3});
    SetMenuItem(DATA.toplevel, 'Expt3ist', id);
    it = findobj(DATA.toplevel,'Tag','RunButton');
    if DATA.inexpt
        set(it,'string','Cancel');
    elseif DATA.optionflags.ts
        set(it,'string','Store');
    else
        set(it,'string','Run');
    end
    
    ot = findobj('tag',DATA.tag.options,'type','figure');
    if length(ot) == 1
        f = fields(DATA.optionflags)
       for j = 1:length(f)
           it = findobj(ot,'Tag',f{j});
           set(it,'value',DATA.optionflags.(f{j}));
       end
    end


 function SetTextItem(top, tag, value, varargin)
 it = findobj(top,'Tag',tag);
 if ~isempty(it)
     set(it,'string',num2str(value));
 end

 function SetMenuItem(top, tag, value, varargin)
if length(value) == 1
     it = findobj(top,'Tag',tag);
     if ~isempty(it)
         set(it,'value',value);
     end
end

function CheckInput(a,b, fig, varargin)
    DATA = get(fig,'UserData');
    ReadFromBinoc(DATA, 'auto');
    if DATA.verbose > 1
    fprintf('Timer read over at %s\n',datestr(now));
    end
    
 
 function DATA = ReadFromBinoc(DATA, varargin)
     global rbusy;
     
     verbose = 0;
     autocall = 0;
     j = 1;
     while j <= length(varargin)
         if strncmpi(varargin{j},'verbose',5)
             verbose = 1;
         elseif strncmpi(varargin{j},'auto',4)
             autocall = 1;
         elseif strncmpi(varargin{j},'reset',5)
             rbusy = 0;
         elseif ischar(varargin{j})
             fprintf('%s',varargin{j});
         end
         j = j+1;
     end
     if rbusy > 0
         fprintf('ReadBinoc busy at %s since %s\n',datestr(now),datestr(rbusy));
         return;
     end
     ts = now;
     rbusy = ts;    
         
     if DATA.verbose >1
     fprintf('%s:',datestr(now,'HH:MM:SS.FFF'))
     end
     fprintf(DATA.outid,'whatsup\n');
     a = fread(DATA.inid,14);
     if strncmp(char(a'),'SENDINGstart1',12)
        a = fread(DATA.inid,14);
        nbytes = sscanf(char(a'),'SENDING%d');
     elseif strncmp(char(a'),'SENDING',7)
         nbytes = sscanf(char(a'),'SENDING%d');
     else
         s = char(a');
         fprintf('No Bytes %s\n',s);
         a = s(end);
         while char(a) ~= 'G' | strcmp(s(end-6:end),'SENDING') == 0
            a = fread(DATA.inid,1);
            s = [s char(a)];
         end
         a = fread(DATA.inid,7);
         nbytes = sscanf(char(a'),'%d');
         fprintf('Read %s\n',s);
     end
     if DATA.verbose > 1
     fprintf('Need %d bytes\n',nbytes);
     end
     if nbytes > 0
         a = fread(DATA.inid,nbytes);
         if DATA.verbose
         fprintf('%s',char(a'));
         fprintf('Read %d bytes took %.2f\n',length(a),mytoc(ts));
         end
         DATA = InterpretLine(DATA,char(a'));
         if isfield(DATA,'toplevel')
             set(DATA.toplevel,'UserData',DATA);
         end
     end
     rbusy = 0;
         
function RunButton(a,b, type)
        DATA = GetDataFromFig(a);
        if type == 1
    fprintf(DATA.outid,'\\expt\n');
        elseif type == 2
    fprintf(DATA.outid,'\\estop\n');
        end
    DATA = ReadFromBinoc(DATA);
    SetGui(DATA);
     
    function TestIO(a,b)
        
        DATA = GetDataFromFig(a);
 %       if DATA.outid > 0
 %           fclose(DATA.outid);
 %       end
        %fclose('all');
        if ~isfield(DATA,'outpipe')
            DATA = OpenPipes(DATA);
        end
        if DATA.outid <= 0
        DATA.outid = fopen(DATA.outpipe,'w');
        end
        fprintf(DATA.outid,'ed+10\n');
%        fclose(DATA.outid);
%        DATA.outid = 0;
        set(DATA.toplevel,'UserData',DATA);
        
        
 
function PenLogPopup(a,b)
  DATA = GetDataFromFig(a);
  cntrl_box = findobj('Tag',DATA.tag.penlog,'type','figure');
  if ~isempty(cntrl_box)
      figure(cntrl_box);
      return;
  end
if length(DATA.winpos{3}) ~= 4
    DATA.winpos{3} = get(DATA.toplevel,'position');
end
cntrl_box = figure('Position', DATA.winpos{3},...
        'NumberTitle', 'off', 'Tag',DATA.tag.penlog,'Name','Penetration Log','menubar','none');
    set(cntrl_box,'UserData',DATA.toplevel);

    nr = 10;
    nc = 6;
    bp = [0.01 0.99-1/nr 1./nc 1./nr];
    uicontrol(gcf,'style','pushbutton','string','Apply', ...
        'units', 'norm', 'position',bp,'value',1,'Tag','Penset','callback',@OpenPenLog);
    bp(1) = bp(1)+bp(3)+0.01;
   uicontrol(gcf,'style','pop','string',DATA.electrodestrings, ...
        'units', 'norm', 'position',bp,'value',1,'Tag','ElectrodeType','callback',{@MenuGui});
 
    
    bp(1) = 0.01;
    bp(2) = bp(2)-1./nr;
    uicontrol(gcf,'style','text','string','#', ...
        'units', 'norm', 'position',bp,'value',1,'Tag','StepSize2');

    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string','0', ...
        'units', 'norm', 'position',bp,'value',1,'Tag','pe','callback',{@TextGui, 'pe'});

    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','text','string','X', ...
        'units', 'norm', 'position',bp,'value',1,'Tag','StepSize2');

    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string','0', ...
        'units', 'norm', 'position',bp,'value',1,'Tag','px','callback',{@TextGui, 'py'});
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','text','string','Y', ...
        'units', 'norm', 'position',bp,'value',1);

    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string','0', ...
        'units', 'norm', 'position',bp,'value',1,'Tag','py','callback',{@TextGui, 'py'});

    bp(1) = 0.01;
    bp(2) = bp(2)-1./nr;
    

function OptionPopup(a,b)
  DATA = GetDataFromFig(a);
  cntrl_box = findobj('Tag',DATA.tag.options,'type','figure');
  if ~isempty(cntrl_box)
      figure(cntrl_box);
      return;
  end
if length(DATA.winpos{2}) ~= 4
    DATA.winpos{2} = get(DATA.toplevel,'position');
end
f = fields(DATA.optionflags);
nc = 4;
nr = ceil(length(f)/nc);
scrsz = get(0,'Screensize');
cntrl_box = figure('Position', DATA.winpos{2},...
        'NumberTitle', 'off', 'Tag',DATA.tag.options,'Name','Options','menubar','none');
    set(cntrl_box,'UserData',DATA.toplevel);
bp = [0.01 0.99-1/nr 1./nc 1./nr];
for j = 1:length(f)
    bp(1) = floor(j/nr) .* 1./nc;
    bp(2) = 1- (rem(j,nr) .* 1./nr);
    uicontrol(gcf,'style','checkbox','string',DATA.optionstrings.(f{j}), ...
        'units', 'norm', 'position',bp,'value',DATA.optionflags.(f{j}),'Tag',f{j},'callback',{@HitToggle, f{j}});
end
    
function StepperPopup(a,b)
  DATA = GetDataFromFig(a);
  cntrl_box = findobj('Tag',DATA.tag.stepper,'type','figure');
if ~isempty(cntrl_box)
    figure(cntrl_box);
    return;
end
scrsz = get(0,'Screensize');
cntrl_box = figure('Position', [10 scrsz(4)-480 300 450],...
        'NumberTitle', 'off', 'Tag',DATA.tag.stepper,'Name','Stepper','menubar','none');
    nr = 6;
    bp = [0.01 0.99-1/nr 0.1 1./nr];
    uicontrol(gcf,'style','pushbutton','string','+', ...
        'Callback', {@Stepper, 1, 1},...
        'units', 'norm', 'position',bp,'value',1);
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','pushbutton','string','-', ...
        'Callback', {@Stepper, -1, 1},...
        'units', 'norm', 'position',bp,'value',1);
    bp(1) = bp(1)+bp(3)+0.01;
    bp(3) = 0.3;
    uicontrol(gcf,'style','pop','string','10|20|50|100|200', ...
        'units', 'norm', 'position',bp,'value',1,'Tag','StepSize1');
    bp(1) = 0.01;
    bp(2) = bp(2) - bp(4)-0.01;
    uicontrol(gcf,'style','pushbutton','string','+', ...
        'Callback', {@Stepper, 1, 2},...
        'units', 'norm', 'position',bp,'value',1);
    bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','pushbutton','string','-', ...
        'Callback', {@Stepper, -1, 2},...
        'units', 'norm', 'position',bp,'value',1);
    bp(1) = bp(1)+bp(3)+0.01;
    bp(3) = 0.3;
    uicontrol(gcf,'style','edit','string','10', ...
        'units', 'norm', 'position',bp,'value',1,'Tag','StepSize2');
    bp(1) = 0.01;
    bp(2) = bp(2) - bp(4)-0.01;
    uicontrol(gcf,'style','edit','string',num2str(DATA.stepperpos), ...
        'units', 'norm', 'position',bp,'value',1,'Tag','StepperPosition');
    set(cntrl_box,'UserData',DATA.toplevel);
    
function Stepper(a,b, step, type)
    DATA = GetDataFromFig(a);
    sfig = get(a,'parent');
    it = findobj(sfig,'Tag','StepSize1');
    DATA.stepsize(1) = Menu2Val(it);
    it = findobj(sfig,'Tag','StepSize2');
    DATA.stepsize(2) = Text2Val(it);
    if step > 0
        s = sprintf('ed+%.3f\n',DATA.stepsize(type));
    else
        s = sprintf('ed-%.3f\n',DATA.stepsize(type));
    end
    fprintf(DATA.outid,'%s\n',s);
    if DATA.penid > 0
        fprintf(DATA.penid,'%s\n',s);
    end
        
    ReadFromBinoc(DATA);
    
function val = Menu2Val(it)
val = NaN;
if isempty(it)
    return;
end
j = get(it(1),'value');
s = get(it(1),'string');
val = str2num(s(j,:));


function OpenPenLog(a,b)
    DATA = GetDataFromFig(a);
    if DATA.penid > 0
        fclose(DATA.penid);
    end
    name = sprintf('/local/%s/pen%d.log',DATA.monkey,DATA.binoc.pe);
    DATA.penid = fopen(name,'a');
    fprintf(DATA.penid,'Penetration %d at %.1f,%.1f Opened %s\n',DATA.binoc.pe,DATA.binoc.px,DATA.binoc.py,datestr(now));
    fprintf(DATA.penid,'Electrode %s\n',DATA.electrodestring);
    set(DATA.toplevel,'UserData',DATA);
    
    
    
function val = Text2Val(it)
val = NaN;
if isempty(it)
    return;
end
s = get(it,'string');
val = str2num(s);

        
function GoToggle(a,b)       
    DATA = GetDataFromFig(a);
    go = get(a,'value');
    if go
      fprintf(DATA.outid,'\\go\n');
    else
      fprintf(DATA.outid,'\\stop\n');
    end
        
function HitToggle(a,b, flag)       
    DATA = GetDataFromFig(a);
%    flag = get(a,'Tag');
    DATA.optionflags.(flag) = get(a,'value');
    s = 'op=';
    f = fields(DATA.optionflags);
    for j = 1:length(f)
        if DATA.optionflags.(f{j})
            s = [s '+' f{j}];
        else
%            s = [s '-' f{j}];
        end
    end
    fprintf('op=0\n%s\n',s);
    fprintf(DATA.outid,'op=0\n%s\n',s);
    ReadFromBinoc(DATA);
 
function TextEntered(a,b)
    DATA = GetDataFromFig(a);
txt = get(a,'string');
if DATA.outid > 0
    fprintf(DATA.outid,'%s\n',txt);
end
fprintf('%s\n',txt);
set(a,'string','');

DATA = ReadFromBinoc(DATA,'from TextEntered ');
a =  get(DATA.txtrec,'string');
n = size(a,1);
if txt(end) == '='
    code = txt(1:end-1);
    txt = ['?' txt '?' num2str(DATA.binoc.(code)')];
end
a(n+1,1:length(txt)) = txt;
set(DATA.txtrec,'string',a);
set(DATA.txtrec,'listboxtop',n+1);

