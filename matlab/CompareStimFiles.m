function CompareStimFiles(stima, stimb)

showall = 0; 
fid = fopen(stima,'r');
a = textscan(fid,'%s','delimiter','\n');
atxt = a{1};
fclose(fid);


fid = fopen(stimb,'r');
a = textscan(fid,'%s','delimiter','\n');
btxt = a{1};
fclose(fid);


for j = 1:length(atxt)
    eid = strfind(atxt{j},'=');
    if isempty(eid)
        eid = 1;
    end
    s = atxt{j}(1:eid(1));
    aval = sscanf(atxt{j}(eid(1)+1:end),'%f');
    if strncmp(s,'qe=',3)
        id = find(strncmp(atxt{j},btxt,length(atxt{j})));
        if isempty(id)
            fprintf('Unmatched %s\n', atxt{j});
        end
    else
        id = find(strncmp(s,btxt,length(s)));
        if isempty(id)
            fprintf('Unmatched %s\n', atxt{j});
        else
        for k = 1:length(id)
            if strfind(atxt{j},'#Fore')
                atype = 1;
            elseif strfind(atxt{j},'#Back')
                atype = 2;
            elseif strfind(atxt{j},'#Choice')
                atype = 3;
            else
                atype = 0;
            end
            if strfind(btxt{id(k)},'#Fore')
                btype = 1;
            elseif strfind(btxt{id(k)},'#Back')
                btype = 2;
            elseif strfind(btxt{id(k)},'#Choice')
                btype = 3;
            else
                btype = 0;
            end
            if atype == btype
                bval = sscanf(btxt{id(k)}(eid(1)+1:end),'%f');
                if showall || length(aval) ~= length(bval) ||  sum(aval ~= bval)
                    fprintf('%s:', regexprep(atxt{j},'#.*',''));
                    cprintf('blue','%s\n',btxt{id(k)});
                end
            else
            end
        end
     end
    end
end