function comparefiles(filea,fileb,len)

fid = fopen(filea);
a = textscan(fid,'%s');
a = a{1};
fclose(fid);

fid = fopen(fileb);
b = textscan(fid,'%s');
b = b{1};
fclose(fid)

missing = [];
for j = 1:length(a)
    l = min([len length(a{j})]);
    id = find(strncmp(a{j},b,l));
    if isempty(id)
        missing = [missing j];
    end
end


fprintf('%d Missing Lines\n',length(missing));
for j = 1:length(missing)
    fprintf('%s\n',a{missing(j)});
end