function DATA = ServoDrive(varargin)
figpos = [1400 400 350 350];
tag = 'Servo Controller';
DATA.verbose = 1;
verbose = [];
j = 1;
while j <= length(varargin)
    if strncmpi(varargin{j},'callback',5)
        j = j+1;
        DATA.callback = varargin{j};
    elseif strncmpi(varargin{j},'position',5)
        j = j+1;
        figpos = varargin{j};
    elseif strncmpi(varargin{j},'quiet',5)
        DATA.verbose = 0;
        verbose = 0;
    elseif strncmpi(varargin{j},'ttyname',3)
        j = j+1;
        DATA.ttyname = varargin{j};
    elseif strncmpi(varargin{j},'verbose',5)
        DATA.verbose = 1;
        verbose = 1;
    end
    j = j+1;
end

[F, isnew] = GetFigure(tag);
if isnew
    set(F,'position',figpos);
    DATA.toplevel = F;
    DATA = SetDefaults(DATA);
    DATA = BuildServoWindow(DATA);
    DATA = OpenServoPort(DATA);
    set(DATA.toplevel,'UserData',DATA);
else 
    DATA = get(F,'UserData');
    if ~isempty(verbose)
        DATA.verbose = verbose;
    end
end

%Things to do after startup
j = 1;
while j <= length(varargin)
    if strncmpi(varargin{j},'readpos',5)
        d = GetCurrentPosition(DATA, 'show');
        DATA.alldepths = [DATA.alldepths d];
        DATA.alltimes = [DATA.alltimes now];
        DATA.position = d;
        set(DATA.toplevel,'UserData',DATA);
    end
    j = j+1;
end

function MoveMicroDrive(a,b,fcn)
DATA = GetDataFromFig(a);

EnableMoveButtons(DATA,'off');
it = findobj(DATA.toplevel,'tag','StepSize');

if strcmp(fcn,'moveto')
    newpos = str2num(get(DATA.setdepth,'string'));
    if abs(newpos - DATA.position) > 1000
        DATA.motorspeed = 0; %force automatic speed setting
    end
    DATA = SetNewPosition(DATA, newpos);
elseif strcmp(fcn,'readposition')
    d = GetCurrentPosition(DATA, 'show');
    DATA.position = d;
elseif strcmp(fcn,'set')
    newpos = str2num(get(DATA.setdepth,'string'));
    SetHomePosition(DATA, newpos);
else
    str = get(it,'string');
    j= get(it,'value');
    step = sscanf(str(j,:),'%d');
    if  strcmp(fcn,'Up')
        step = -step;
    end
    if DATA.verbose > 1
        fprintf('Moving %d\n',step);
    end
    DATA = ChangePosition(DATA, step);
end
EnableMoveButtons(DATA,'on');
set(DATA.toplevel,'UserData',DATA);


function DATA = BuildServoWindow(DATA)
F = DATA.toplevel;
nr = 7;
nc=6;

set(F,'menubar','none');
im = load('/local/Images/icons/DownArrow.mat');
im.x = im.x-min(im.x(:));
im.x = im.x./max(im.x(:));
[ii,ij] = ind2sub(size(im.x),find(im.x < 0.04));

im.x = 1-im.x;
upx = flipud(im.x);
bcolor = get(gcf,'color');
im.cdata = repmat(upx,[1 1 3]);
im.cdata(find(im.cdata > 0.96)) = mean(bcolor);
bp = [0.1 0.7 0.3 0.2];
uicontrol(gcf,'style','pushbutton','string','Up', ...
     'foregroundcolor','w', ...
   'Callback', {@MoveMicroDrive, 'Up'}, 'Tag','UpArrow',...
   'backgroundcolor',bcolor,...
        'units', 'norm', 'position',bp,'value',1,'cdata',im.cdata);


bp = [0.05 0.4 0.3 0.15];
h = uicontrol(gcf,'style','popupmenu','string','10uM|20uM|25uM|50uM|100uM|200uM|Set Custom', ...
        'units', 'norm', 'position',bp,'value',1,...
        'Tag','StepSize','fontsize',18,'fontweight','bold','callback',@SetStepSize);

    bp(2) = 0.2;
    bp(1) = bp(1)+bp(3)+0.05;
bp(3) = 0.15;
uicontrol(gcf,'style','pushbutton','string','Set', ...
   'Callback', {@MoveMicroDrive, 'set'}, 'Tag','SetPosition',...
   'fontsize',18,'fontweight','bold',...
        'units', 'norm', 'position',bp,'value',1);
bp(1) = bp(1)+bp(3);
bp(3) = 0.2;
DATA.setdepth = uicontrol(gcf,'style','edit','string','0', ...
   'fontsize',18,'fontweight','bold',...
        'units', 'norm', 'position',bp,'value',1);
bp(1) = bp(1)+bp(3);
bp(3) = 0.2;
uicontrol(gcf,'style','pushbutton','string','Move', ...
   'Callback', {@MoveMicroDrive, 'moveto'}, 'Tag','MoveButton',...
   'fontsize',18,'fontweight','bold',...
        'units', 'norm', 'position',bp,'value',1);
% 
% bp = [0.5 0.2 0.45 0.15];
%     uicontrol(gcf,'style','pushbutton','string','Reopen', ...
%    'Callback', {@MoveMicroDrive, 'reopen'}, ...
%    'fontsize',18,'fontweight','bold',...
%         'units', 'norm', 'position',bp,'value',1);

bp = [0.1 0.21 0.3 0.2];
im.cdata = repmat(im.x,[1 1 3]);
im.cdata(find(im.cdata > 0.96)) = mean(bcolor);
h = uicontrol(gcf,'style','pushbutton','string','Down', ...
   'Callback', {@MoveMicroDrive, 'down'}, 'Tag','DownArrow',...
        'units', 'norm', 'position',bp,'value',1,...
        'foregroundcolor','w', 'backgroundcolor',bcolor,'cdata',im.cdata);
    bp(1) = bp(1)+bp(3);
    bp(1) = bp(1)+bp(3);
    bp(3) = 0.1;

bp = [0.05 0.1 0.9 0.15];
h = uicontrol(gcf,'style','check','string','Emergency Stop', ...
        'units', 'norm', 'position',bp,'value',0,...
        'Tag','EmergencyStop','fontsize',18,'fontweight','bold','callback',@StopMotor);

bp = [0.3 0.01 0.4 0.1];
DATA.depthlabel = uicontrol(gcf,'style','text','string','0 uM', ...
        'units', 'norm', 'position',bp,'value',1,'backgroundcolor',bcolor,'foregroundcolor',[0.8 0.3 0.2],...
        'fontsize',24,'fontweight','bold');
mn = uimenu(F,'Label','Close','callback',@CloseServoPort);
mn = uimenu(F,'Label','Reopen','callback',@OpenServoPort);
mn = uimenu(F,'Label','Stop','callback',@StopMotor);
mn = uimenu(F,'Label','Options');
sm = uimenu(mn,'Label','Read Position','callback',{@MoveMicroDrive, 'readposition'},'accelerator','R');
sm = uimenu(mn,'Label','Plots');
uimenu(sm,'Label','Full History','tag','FullHistory','checked','on','callback',@SetMotorPlot);
uimenu(sm,'Label','Last Move','tag','LastMove','callback',@SetMotorPlot);
uimenu(sm,'Label','Speed','tag','MoveSpeed','callback',@SetMotorPlot);
uimenu(sm,'Label','None','tag','None','callback',@SetMotorPlot);
sm = uimenu(mn,'Label','Speed');
uimenu(sm,'Label','Normal (1 mm/s)','tag','Normal','Checked','on','callback',@SetMotorSpeed);
uimenu(sm,'Label','Fast (5 mm/s)','tag','Fast','callback',@SetMotorSpeed);
uimenu(sm,'Label','Slow (0.5 mm/s)','tag','Slow','callback',@SetMotorSpeed);
uimenu(sm,'Label','Very Slow (0.1 mm/s)','tag','VSlow','callback',@SetMotorSpeed);
uimenu(sm,'Label','Automatic','tag','Auto','callback',@SetMotorSpeed);
uimenu(sm,'Label','Custom','tag','Custom','callback',@SetMotorSpeed);
sm = uimenu(mn,'Label','Verbose','callback',@SetOption,'tag','verbose');
sm = uimenu(mn,'Label','Show Buttons','callback',{@MenuOptions,'showbuttons'});
if DATA.verbose
    set(sm,'checked','on');
end

set(gca,'position',[0.4 0.4 0.6 0.6],'xtick',[],'ytick',[]);
set(DATA.toplevel,'UserData',DATA);

function EnableMoveButtons(DATA,state)

tags = {'DownArrow', 'UpArrow' 'MoveButton'};
for j = 1:length(tags)
    it = findobj(DATA.toplevel,'tag',tags{j});
    if ~isempty(it)
        set(it,'visible',state);
    end
end

function MenuOptions(a,b,fcn)
   DATA = GetDataFromFig(a);
if strcmp(fcn,'showbuttons')
    EnableMoveButtons(DATA,'on');
end


function SetOption(a,b)
   DATA = GetDataFromFig(a);
   tag = get(a,'tag');
   DATA.(tag) = ~DATA.(tag);
   if DATA.(tag) 
       set(a,'checked','on');
   else
       set(a,'checked','off');
   end
   set(DATA.toplevel,'UserData',DATA);
   
function SetMotorPlot(a,b)

   DATA = GetDataFromFig(a);
   tag = get(a,'tag');
   DATA.plottype = tag;
   SetMenuCheck(a,'exclusive');
   PlotDepths(DATA);
   set(DATA.toplevel,'UserData',DATA);

function SetMotorSpeed(a,b)
DATA = GetDataFromFig(a);
tag = get(a,'tag');

if strcmp(tag,'Normal')
    DATA.motorspeed = 1.0;
elseif strcmp(tag,'Slow')
    DATA.motorspeed = 0.5;
elseif strcmp(tag,'Fast')
    DATA.motorspeed = 5;
elseif strcmp(tag,'VSlow')
    DATA.motorspeed = 0.1;
elseif strcmp(tag,'Auto')
    DATA.motorspeed = 0;
elseif strcmp(tag,'Custom')
    defaultanswer{1} = num2str(DATA.customspeed);
    str = inputdlg('Step Size in uM/sec','Custom',1,defaultanswer);
    if isempty(str)
       return;
    end
    step = str2num(str{1});
    str = sprintf('Custom (%d uM/s)',step);
    set(a,'Label',str);
    DATA.motorspeed = step./1000; %Custom is in uM/sec, not mm/sec
    DATA.customspeed = step./1000;
end
if DATA.motorspeed > 0
    ispeed = round(DATA.motorspeed.*DATA.speedscale.*DATA.stepscale/1000);
    fprintf('New Speed %.3f mm/sec  = %.0f RPM\n',DATA.motorspeed,ispeed);
    fprintf(DATA.sport,'SP%.0f\n',ispeed);
    DATA.motorspeed = ispeed;
end
SetMenuCheck(a,'exclusive');
set(DATA.toplevel,'UserData',DATA);

function CloseServoPort(a,b)
DATA = GetDataFromFig(a);
%tell verg first
if ~isempty(DATA.callback)
    feval(DATA.callback{:}, 'close');
end

if isfield(DATA, 'sport');
    fclose(DATA.sport);
    delete(DATA.sport);
end
close(DATA.toplevel);

function SetStepSize(a,b)

DATA = GetDataFromFig(a);
val = get(a,'value');
s = get(a,'string');
if strncmp(s(val,:),'Set Custom',8)
    step = DATA.customstep;
    defaultanswer{1} = num2str(step);
    str = inputdlg('Step Size (uM)','Custom',1,defaultanswer);
    if ~isempty(str)
    step = str2num(str{1});
    str = sprintf('%d (Custom)',step);
    DATA.customstep = step;
    DATA.step = step;
    s(val+1,1:length(str)) = str;
    set(a,'string',s,'value',val+1);
    end
else
    DATA.step = sscanf(s(val,:),'%d');
end
set(DATA.toplevel,'UserData',DATA);

function DATA = SetDefaults(DATA)
DATA.position = 0;
DATA.step = 0;
DATA.customstep = 0;
DATA.stepscale = 65.6;
DATA.speedscale = 15000;
DATA.customspeed = 1; %also default
DATA.motorspeed = round(DATA.customspeed.* DATA.speedscale.*DATA.stepscale/1000);

DATA.motorid = -1; %< 0 means dont set id
DATA.alldepths = [];
DATA.alltimes = [];
if ~isfield(DATA,'ttyname')
    DATA.ttyname = '/dev/tty.USA49Wfa1212P1.1';
end
if ~isfield(DATA,'callback')
    DATA.callback = [];
end
if ~isfield(DATA,'maxrange')
    DATA.maxrange = 3000000; %c. 30mm
end
if ~isfield(DATA,'plottype')
    DATA.plottype = 'FullHistory';
end

function SetHomePosition(DATA, pos)

if DATA.motorid >= 0
	fprintf(DATA.sport,sprintf('%dHO%.0f\n', DATA.motorid, pos .* DATA.stepscale));
else
	fprintf(DATA.sport,sprintf('HO%.0f\n', pos .* DATA.stepscale));
end
    set(DATA.setdepth,'string',sprintf('%.0f',pos));
    set(DATA.depthlabel,'string',sprintf('%.0f',pos));
    
    
function DATA = ChangePosition(DATA, step)

d = GetCurrentPosition(DATA,'show');
DATA = SetNewPosition(DATA,d+step);

function d = GetCurrentPosition(DATA, varargin)

showdepth = 0;
j = 1;
while j <= length(varargin)
    if strncmpi(varargin{j},'show',4)
        showdepth = 1;
    end
    j = j+1;
end

if DATA.motorid >= 0
fprintf(DATA.sport,sprintf('%dPOS\n',DATA.motorid));
else
fprintf(DATA.sport,sprintf('POS\n'));
end
%fprintf(DATA.sport,sprintf('0POS\n'));
pause(0.01);
s = ReadLine(DATA.sport);
d = sscanf(s,'%d');
if DATA.verbose
    fprintf('From uDrive:%s\n',s);
end
d = d./DATA.stepscale;
if showdepth
    set(DATA.depthlabel,'string',sprintf('%.0f uM',d));
end

function StopMotor(a,b)
DATA = GetDataFromFig(a);
if DATA.motorid >= 0
fprintf(DATA.sport,'%dDI\n',DATA.motorid);
else
fprintf(DATA.sport,'DI\n');
end

function DATA = SetNewPosition(DATA, pos)

newpos = pos .* DATA.stepscale;
pause(0.01);
d = NaN;

if DATA.motorid >= 0
    fprintf(DATA.sport,'%dPOS\n',DATA.motorid);
else
    fprintf(DATA.sport,'POS\n');
end
    pause(0.01);
s = ReadLine(DATA.sport);
if isempty(s)
    uiwait(warndlg(sprintf('MicroDrive Not Responding'),'Microdrive Error','modal'));
    return;
end
d = sscanf(s,'%d');
margin = 20 * DATA.stepscale; %allow for up to 20uM of noise
step = newpos-d;
edur = 0.1 + 2 .* abs(newpos-d) ./(DATA.motorspeed .* DATA.stepscale); %estimated duration
if edur > 600
    edur = 100;
end
if isempty(edur)
    edur = 1;
end
stopui = findobj(DATA.toplevel,'tag','EmergencyStop');
set(stopui,'value',0);
drawnow;
if DATA.motorspeed == 0 %automatic speed
    dd = abs(d-newpos)./DATA.stepscale; %step in uM
    if dd > 1000
        speed = 0.1;
    elseif dd > 50
        speed = 0.5;
    else
        speed = 1;
    end
    ispeed = round(speed.*DATA.stepscale.*DATA.speedscale/1000);
    if DATA.verbose
        fprintf('Speed %.3fmm/sec = %.0f rpm\n',speed,ispeed);
    end
    fprintf(DATA.sport,'SP%.0f\n',ispeed);
end

if DATA.verbose
    fprintf('Requesting %.0f->%.0f (~%.2f sec)\n',d,newpos,edur);
end

if abs(newpos) > DATA.maxrange
    uiwait(warndlg(sprintf('Requested Poition out of Range - Use Set button'),'Microdrive Error','modal'));
    return;
end

if DATA.motorid >= 0
    fprintf(DATA.sport,'%dEN\n',DATA.motorid);
    fprintf(DATA.sport,sprintf('%dLA%.0f\n',DATA.motorid,newpos));
    pause(0.01);
    fprintf(DATA.sport,'%dM\n',DATA.motorid);
else
    fprintf(DATA.sport,'EN\n');
    fprintf(DATA.sport,sprintf('LA%.0f\n',newpos));
%    s = sprintf('LR%.0f\n',newpos-d)
%    fprintf(DATA.sport,s);
    pause(0.01);
    fprintf(DATA.sport,'M\n');
end
pause(0.01);
ts = now;
npost = 0;
newd(1) = d;

%Determine a range of acceptable postions given
%the step. So in case motor moves wrong way, or overshoots
%final position without ever having a small error, it is stopped.
startpos = d;
if step < 0
    minpos = startpos+step-margin;
    maxpos = startpos+margin;
else
    minpos = startpos-margin;
    maxpos = startpos+step+margin;
end
j = 2;
while npost < 2
    if DATA.motorid >= 0
        fprintf(DATA.sport,sprintf('%dPOS\n',DATA.motorid));
    else
        fprintf(DATA.sport,sprintf('POS\n'));
    end
    s = ReadLine(DATA.sport);
    if strcmp(s,'OK') %in case Controller is in verbose state
        if DATA.verbose
            fprintf('Returned %s\n',s);
        end
        s = ReadLine(DATA.sport);
    end
    ts(j) = now;
    if ~isempty(s)
        try
            newd(j) = sscanf(s,'%d');
            set(DATA.depthlabel,'string',sprintf('%.0f uM',newd(j)./DATA.stepscale));
            drawnow;
            poserr = abs(newd(j)-newpos);
            if poserr < 20
                npost = npost+1;
            end
            if edur > 2
                PlotDepths(DATA, ts, newd);
            end
        catch
            fprintf('Returned %s\n',s);
        end
    else
        newd(j) = 0;
    end
    
    if mytoc(ts(1)) > edur+1
        fprintf(DATA.sport,'DI\n');
       npost = 2;
       F = gcf;
       uiwait(warndlg(sprintf('Only Moved to %.3f',newd(end)./(DATA.stepscale.*1000)),'Microdrive Error','modal'));
       figure(F);
    elseif newd(j) < minpos
        fprintf(DATA.sport,'DI\n');
       F = gcf;
       uiwait(warndlg(sprintf('Postion %.3f min allowd %.3f',newd(end)./(DATA.stepscale.*1000),minpos./(DATA.stepscale.*1000)),'Microdrive Error','modal'));
       figure(F);
    elseif newd(j) > maxpos
        fprintf(DATA.sport,'DI\n');
       F = gcf;
       uiwait(warndlg(sprintf('Postion %.3f Max allowed %.3f',newd(end)./(DATA.stepscale.*1000),maxpos./(DATA.stepscale.*1000)),'Microdrive Error','modal'));
       figure(F);
    else
        stop = get(stopui,'value');
        if stop
            npost = 10;
        end
    end
    j = j+1;
end

DATA.position = newd(end)./DATA.stepscale;

dur = (ts(end)-ts(1)).*(24 * 60 *60);
dp = diff(newd);
id = find(abs(dp) > mean(abs(dp))/2); 
if length(id) > 1
    rate = (newd(id(end)+1) - newd(1))./(ts(id(end)+1) - ts(1));
    rate = rate ./ (DATA.stepscale * 1000 .* 60 * 60 * 24 );
else
    rate = (newd(end)-newd(1))/(dur.*DATA.stepscale.*1000);
end

if DATA.verbose
    fprintf('End pos %s took %.2f (%.2f) rate %.2f mm/sec\n',s,dur,edur,rate);
end

if DATA.motorid >= 0
    fprintf(DATA.sport,'%dDI\n',DATA.motorid);
else
    fprintf(DATA.sport,'DI\n');
end
pause(0.01);
DATA = PlotDepths(DATA, ts, newd);

if ~isempty(DATA.callback)
    feval(DATA.callback{:}, newd(end)./DATA.stepscale);
end


function DATA = PlotDepths(DATA, ts, newd)

if nargin == 1
    ts = DATA.newtimes;
    newd = DATA.newdepths;
elseif nargin == 3
    DATA.newtimes = ts;
    DATA.newdepths = newd./DATA.stepscale;
    DATA.alltimes = [DATA.alltimes ts];
    DATA.alldepths = [DATA.alldepths DATA.newdepths];    
end

if ~strcmp(DATA.plottype,'None')
    if strcmp(DATA.plottype,'LastMove')
        ts = ts-ts(1);
        y =newd./DATA.stepscale;
        plot(ts, y);
        set(gca,'xtick',[],'ytick',[],'ydir','reverse');
        xl = [min(ts) max(ts)];
        yl = [min(y) max(y)];
    elseif strcmp(DATA.plottype,'MoveSpeed')
        ts = ts-ts(1);
        k = 24 * 60 * 60 ./DATA.stepscale;
        dt = diff(ts) .* 24 * 60 * 60;
        y = diff(newd)./(dt .*DATA.stepscale);
        plot(ts(2:end), y);
        set(gca,'xtick',[],'ytick',[],'ydir','normal');
        xl = [min(ts) max(ts)];
        yl = minmax(y);
    else
        plot(DATA.alltimes, DATA.alldepths);
        set(gca,'xtick',[],'ytick',[],'ydir','reverse');
        xl = [min(DATA.alltimes) max(DATA.alltimes)];
        yl = [min(DATA.alldepths) max(DATA.alldepths)];
    end
    
    if diff(yl) <= 0
        return;
    end
    tdur = diff(xl).*24; %hours
    tlabel = 'hr';
    if tdur < 1
        tdur = tdur .* 60;
        tlabel = 'min';
    end
    if tdur < 1
        tdur = tdur .* 60;
        tlabel = 'sec';
    end
    %N.B. ydir is reversed (deep == down)
    axis([xl yl]);
    text(xl(2),yl(1),sprintf('%.1f%s',tdur,tlabel),'horizontalalignment','right','verticalalignment','top');
    if strcmp(DATA.plottype,'MoveSpeed')
        text(xl(1),yl(2),sprintf('%.1fuM/sec',max(yl)),'horizontalalignment','left','verticalalignment','bottom');
        text(xl(1),yl(1),sprintf('%.1fuM/sec',min(yl)),'horizontalalignment','left','verticalalignment','top');
    else
        text(xl(1),yl(2),sprintf('%.1fuM',diff(yl)),'horizontalalignment','left','verticalalignment','bottom');
    end
else
    delete(get(gca,'children'));
    bc = get(gcf,'color');
    set(gca,'color',bc,'box','off','xcolor',bc,'ycolor',bc);
end



function s = ReadLine(port)

waitforbytes = 0;
ts = now;
while get(port,'BytesAvailable') == 0 && waitforbytes
    pause(0.001);
end
s = fscanf(port,'%s');


function DATA = OpenServoPort(a,b)
 
DATA = GetDataFromFig(a);
x = instrfind('type','serial');
delete(x);
DATA.sport = serial(DATA.ttyname,'BaudRate',9600);
fopen(DATA.sport);
fprintf(DATA.sport,'ANSW1\n'); %?0 would stop unwanted "OK"
fprintf(DATA.sport,'SOR0\n');
fprintf(DATA.sport,'NET0\n');
fprintf(DATA.sport,'BAUD%d\n',9600);
fprintf(DATA.sport,'SP%d\n',DATA.motorspeed);

fprintf(DATA.sport,'APL1\n');
fprintf(DATA.sport,'LL%.0f\n',-DATA.maxrange);
fprintf(DATA.sport,'LL%.0f\n',DATA.maxrange);
fprintf(DATA.sport,'APL1\n');
if DATA.motorid >= 0
    fprintf(DATA.sport,'%dEN\n',DATA.motorid);
else
    fprintf(DATA.sport,'EN\n');
end


DATA.position = GetCurrentPosition(DATA,'show');
fprintf(DATA.sport,'DI\n');
set(DATA.toplevel,'UserData',DATA);
