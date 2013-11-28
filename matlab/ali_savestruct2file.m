function err = ali_savestruct2file(m, filename)
% gets a struct and a filename (full path to the file) and
% saves a string representation of the struct into the text file. this is
% primarily mean to to be used for binoc (stim) files, and puts each
% field-value pair in one line with a = between them without any spaces.
    mapstr = '';
    fs = [fields(m)];
    for ki = 1:length(fs), 
        if strcmp(num2str(m.(fs{ki})), '')
            mapstr = [mapstr, fs{ki}, '=', m.(fs{ki}), '\n'];
        else
            mapstr = [mapstr, fs{ki}, '=', num2str(m.(fs{ki})), '\n'];
        end
    end
    fid = fopen(filename, 'w');
    fprintf(fid, mapstr);
    fclose(fid);
    err = '';
    