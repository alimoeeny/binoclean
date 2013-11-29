function comparefiles(filea,fileb,len)
% comparefiles(filea,fileb,len)
% finde lines in file a that are not in file b
%regarless of order
a = scanlines(filea);
original = a;
b = scanlines(fileb);

for j = 1:length(a)
    a{j} = regexprep(a{j},'\s+#.*','');
end
for j = 1:length(b)
    b{j} = regexprep(b{j},'\s+#.*','');
end
missing = [];
for j = 1:length(a)
    l = min([len length(a{j})]);
    id = find(strncmp(a{j},b,l));
    if isempty(id) && a{j}(1) ~= '#'
        l = strfind(a{j},'=');
        if ~isempty(l)
            l = l(1);
        else
            l = 2;
        end
        ok = 0;
        astr = a{j};
        xid = find(strncmp(a{j},b,l));
        if ~isempty(xid)
            bstr = b{xid};
            x = sscanf(a{j}(l+1:end),'%f');
            y = sscanf(b{xid(1)}(l+1:end),'%f');
            if x == y
                ok = 1;
            else
                ok = 0;
            end
        end
        if ok == 0
            missing = [missing j];
        end
    end
end


fprintf('%d Missing Lines\n',length(missing));
for j = 1:length(missing)
    fprintf('%s',a{missing(j)});
    id = find(strncmp(a{missing(j)},b,2));
    if ~isempty(id)
        fprintf('   Nearest %s',b{id(1)});
    end
    fprintf('\n');
end