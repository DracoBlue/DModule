-- See Copyright Notice in core\LICENSE.txt
require("lfs") 

function file_get_contents(name) 
	local file = io.open(name, "r")
	if (file) then
		local data = file:read("*a")
		file:close()
		return data
	end
	return nil
end

function file_put_contents(name,content)
	local file = io.open(name,"w+")
	if (file) then
		file:write(content)
		file:close()
		return true
	end
	return false
end

function print_r (t, indent, done)
  done = done or {}
  indent = indent or ''
  local nextIndent -- Storage for next indentation value
  for key, value in pairs (t) do
    if type (value) == "table" and not done [value] then
      nextIndent = nextIndent or
          (indent .. string.rep(' ',string.len(tostring (key))+2))
          -- Shortcut conditional allocation
      done [value] = true
      print (indent .. "[" .. tostring (key) .. "] => Table {");
      print  (nextIndent .. "{");
      print_r (value, nextIndent .. string.rep(' ',2), done)
      print  (nextIndent .. "}");
    else
      print  (indent .. "[" .. tostring (key) .. "] => " .. tostring (value).."")
    end
  end
end

function explode(div,str) -- abuse to: http://richard.warburton.it
	if (str==nil) then return {} end
	local pos,arr = 0,{}
	for st,sp in function() return string.find(str,div,pos,true) end do -- for each divider found
		table.insert(arr,string.sub(str,pos,st-1)) -- Attach chars left of current divider
		pos = sp + 1 -- Jump past current divider
	end
	table.insert(arr,string.sub(str,pos)) -- Attach chars right of last divider
	return arr
end

table.print = (function()
	local that = {}
	local serialize_value = function (val)
		if (type(val) == "string") then
			return tostring(string.format("\"%s\"", tostring(val)))
		elseif (type(val) == "number" or type(val) == "boolean" or val == nil) then
			return tostring(val)
		end
	end
	
	that.table_print = function(tt, indent, done)
		done = done or {}
		indent = indent or 0
		if type(tt) == "table" then
			local sb = {}
			for key, value in pairs (tt) do
				table.insert(sb, string.rep (" ", indent)) -- indent it
				if type (value) == "table" and not done [value] then
					done [value] = true
					table.insert(sb, string.format("%s = {\n", tostring (key)))
					table.insert(sb, that.table_print (value, indent + 2, done))
					table.insert(sb, string.rep (" ", indent)) -- indent it
					table.insert(sb, "},\n");
				elseif "number" == type(key) then
					table.insert(sb, string.format("%s,\n", serialize_value(value)))
				else
					table.insert(sb, string.format("%s = %s,\n", tostring (key), serialize_value(value)))
				end
			end
			return table.concat(sb)
		else
			return tt .. "\n"
		end
	end
	
	return that.table_print
end)()


function trim(s)
	return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

function str_replace(old,new,str)
	return table.concat(explode(old,str),new)
end

function readLuaCfgFile(filename) 
	local contents = file_get_contents(filename)
	
	if (not contents) then
		return nil
	end
	
	return assert(loadstring("return "..contents))()
end

function saveLuaCfgFile(filename, cfg) 
	file_get_contents(filename, "{\n"..table.print(cfg,4 ).."}")
end


function readCfgFile(filename) 
	local cfg={}
	for pos,line in pairs(explode("\n",file_get_contents(filename))) do 
		local iscomment=string.match(line,"^%s*(#).*")
		line=trim(line)
		if (line=="") then
			-- empty line, ignore it for now.
		elseif (not iscomment) then
			varia,value=string.match(line,"(.*)=(.*)")
			if (varia) then 
				cfg[varia]=value
			else
				print("")
				print("           (read) BROKEN cfg-Line in file "..filename..":"..pos)
				print("")
			end
		end
	end
	return cfg
end

function setCfgFileLine(filename,linename,newval)
	local newlines = {}
	local wasset = false
	for pos,line in pairs(explode("\n",file_get_contents(filename))) do 
		local iscomment=string.match(line,"^%s*(#).*")
		line=trim(line)
		if (line=="") then
			table.insert(newlines,"")
			-- empty line, ignore it for now.
		elseif (not iscomment) then
			varia,value=string.match(line,"(.*)=(.*)")
			if (varia) then 
				if (varia==linename) then
					table.insert(newlines,linename.."="..newval)
					wasset = true
				else
					table.insert(newlines,line)
				end
			else
				-- ok broken line, we'll ignore it
				table.insert(newlines,line)
			end
		else
			-- ok comment, lets add it:
			table.insert(newlines,line)
		end
	end
	if (not wasset) then
		table.insert(newlines,linename.."="..newval)
	end
	file_put_contents(filename,table.concat(newlines,"\n"))
end

function unsetCfgFileLine(filename,linename)
	local newlines = {}
	for pos,line in pairs(explode("\n",file_get_contents(filename))) do 
		local iscomment=string.match(line,"^%s*(#).*")
		line=trim(line)
		if (line=="") then
			table.insert(newlines,"")
			-- empty line, ignore it for now.
		elseif (not iscomment) then
			varia,value=string.match(line,"(.*)=(.*)")
			if (varia) then 
				if (varia==linename) then
					-- ignore that line, since its the one we want to unset
				else
					table.insert(newlines,line)
				end
			else
				-- ok broken line, we'll ignore it
				table.insert(newlines,line)
			end
		else
			-- ok comment, lets add it:
			table.insert(newlines,line)
		end
	end
	file_put_contents(filename,table.concat(newlines,"\n"))
end

function readProjectCfgFile(project_name)
	local cfg = readLuaCfgFile(".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project_name..directory_delimiter.."project.cfg")
	if (not cfg) then
		cfg=readCfgFile(".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project_name..directory_delimiter.."settings.cfg")
		
		if (cfg.moduledirectories) then
			cfg.moduledirectories = explode(" ",cfg.moduledirectories)
		end
		if (cfg.modules) then
			cfg.modules = explode(" ",cfg.modules)
		end
		if (cfg.languages) then
			cfg.languages = explode(" ",cfg.languages)
		end
		if (cfg.debug_events == "true") then
			cfg.debug_events = true
		elseif (cfg.debug_events == "false") then
			cfg.debug_events = false
		end 
		if (cfg.summary_file == "true") then
			cfg.summary_file = true
		elseif (cfg.summary_file == "false") then
			cfg.summary_file = false
		end 
	end
	if (cfg.moduledirectories==nil) then
		cfg.moduledirectories={"modules.c","modules.d","modules"}
	end
	if (cfg.languages==nil) then
		cfg.languages={"english"}
	end
	if (cfg.defaultlanguage==nil) then
		cfg.defaultlanguage={"english"}
	end
	if (cfg.summary_file==nil) then
		cfg.summary_file=false
	end
	if (cfg.modules==nil) then
		cfg.modules={}
	end
	return cfg
end

function onError(dreason,file,line)
	onProblem(1,dreason,file,line)
end

function onWarning(dreason,file,line)
	onProblem(2,dreason,file,line)
end

function onProblem(dtyp,dreason,file,line)
	table.insert(project.problems,{typ=dtyp,reason=dreason,file=file,line=line})
	if (dtyp == 2) then
		print("  Warning: "..dreason)
		print("  	"..line..":"..file)
		print("")
	end
	if (dtyp == 1) then
		print("  Error: Can't build '"..project.name.."' because:")
		print("")
		print("  	"..dreason)
		print("")
		print("  	file: "..file)
		print("  	line: "..line)
		print("")
		print("")
		file_put_contents("..\\..\\projects\\"..project.name.."\\build.error.log",dreason)
		os.exit()
	end
end

project={}
project.name=arg[1]
project.problems={}
directory_delimiter = "\\"
rm_cmd_pre = "rmdir "
rm_cmd_post = "  /S /Q"
copy_cmd_pre = "copy "
del_cmd_pre = "del "
plugins_suffix = ""
if (arg[2] == "linux") then
	directory_delimiter = "/"
	rm_cmd_pre = "echo "
	rm_cmd_post = " -rf"
	copy_cmd_pre = "cp "
	del_cmd_pre = "rm "
	plugins_suffix = ".so"
end
if (arg[2] == "wine") then
	plugins_suffix = ".so"
end
