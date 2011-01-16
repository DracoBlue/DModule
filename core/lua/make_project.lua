-- See Copyright Notice in core\LICENSE.txt
require("utils")
--[[
	Script-Config
]]
PRINT_R_WEIGHTS = false
PRINT_R_ALL = false

project.cfg = readProjectCfgFile(project.name)
project.moduledirectories={}
for _,k in pairs(project.cfg.moduledirectories) do
	if (trim(k)~="") then
		table.insert(project.moduledirectories,k)
	end
end

project.events={}
project.translations={}
project.languages={}
project.rawmodules={}
project.nocallbackdefinitions={}

-- Clean the plugins folder
os.execute(rm_cmd_pre..".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."build"..directory_delimiter.."plugins"..rm_cmd_post,true)
lfs.mkdir(".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."build"..directory_delimiter.."plugins")

print("  Building '"..project.name.."' ... ")

-- Get all selected modules

local need_to_parse_modules = {}
local need_to_parse_module_list = ""
local needed_modules = {}
local needed_modules_line = {}

for _,k in pairs(project.cfg.modules) do
	if (trim(k)~="") then
		need_to_parse_module_list = trim(k) .. " " .. need_to_parse_module_list
		needed_modules[k]="settings.cfg"
		needed_modules_line[k]="-"
	end
end
need_to_parse_modules = explode(" ",need_to_parse_module_list)
need_to_parse_modules[table.getn(need_to_parse_modules)]=nil

local pos = 0
while (pos<table.getn(need_to_parse_modules)) do
	pos = pos + 1
	local modname = need_to_parse_modules[pos]
	project.rawmodules[pos]={}
	project.rawmodules[pos].name=modname
	project.rawmodules[pos].depends_light={}
	project.rawmodules[pos].values={}
	project.rawmodules[pos].plugins={}
	local folder_selected=nil;
	for _,moduledir in pairs(project.moduledirectories) do
		if (nil ~= file_get_contents(".."..directory_delimiter..".."..directory_delimiter..moduledir..directory_delimiter..modname..directory_delimiter..modname..".dmodule.inc")) then
			folder_selected = moduledir
		end
	end
	if (folder_selected==nil) then
		local whatfile = needed_modules[modname]
		if (needed_modules[modname]~="settings.cfg") then
			whatfile = needed_modules[modname]..".depends.txt"
		end 
		onError("Can't find module '"..modname.."', required by '"..needed_modules[modname].."'!",whatfile,needed_modules_line[modname])
	end
	project.rawmodules[pos].folder=folder_selected
	
	local depends = file_get_contents(".."..directory_delimiter..".."..directory_delimiter..folder_selected..directory_delimiter..modname..directory_delimiter..modname..".depends.txt") 
	if (nil ~= depends) then
		for line,depend_modname in pairs(explode("\n",depends)) do
			if (trim(depend_modname) ~= "") then
				depend_modname = trim(depend_modname)
				if (string.sub(depend_modname,1,1) == '+') then
					depend_modname = string.sub(depend_modname,2)
				else
					if (needed_modules[trim(depend_modname)]==nil) then
						table.insert(need_to_parse_modules,trim(depend_modname))
						needed_modules[trim(depend_modname)]=modname
						needed_modules_line[trim(depend_modname)]=line
					end
				end
				table.insert(project.rawmodules[pos].depends_light,depend_modname)
			end
		end
	end
end

if (project.cfg.summary_file) then
	local summary_file = {}
	table.insert(summary_file, '{ "modules": [')
	for pos, raw_module in pairs(project.rawmodules) do
		if (pos > 1) then
			table.insert(summary_file, ',')
		end
		local modname = raw_module.name
		table.insert(summary_file, '{"name":"' .. modname .. '", "folder":"' .. raw_module.folder .. '"')
		if (needed_modules[modname] ~= "settings.cfg") then
			table.insert(summary_file, ',"required_by":"' .. needed_modules[modname] .. '"')
		end
		if (#raw_module.depends_light > 1) then
			table.insert(summary_file, ',"depends":["' .. table.concat(raw_module.depends_light, '","') .. '"]')
		end
		table.insert(summary_file, '}')
	end
	table.insert(summary_file, ']}')
	file_put_contents(".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."build"..directory_delimiter.."build_summary.json",table.concat(summary_file,""))
end

project.events["WritePlayerLanguage"]={}
project.events["WritePlayerLanguage"].has_return=true
project.events["WritePlayerLanguage"].name="WritePlayerLanguage"
project.events["WritePlayerLanguage"].module=""
project.events["WritePlayerLanguage"].params_def="id,newval"
project.events["WritePlayerLanguage"].params="id,newval"
project.events["WritePlayerLanguage"].lastline="A_PlayerLanguage[id]=newval;\n	return 1;"
project.events["ReadPlayerLanguage"]={}
project.events["ReadPlayerLanguage"].has_return=true
project.events["ReadPlayerLanguage"].has_return_text="A_PlayerLanguage[id]";
project.events["ReadPlayerLanguage"].name="ReadPlayerLanguage"
project.events["ReadPlayerLanguage"].module=""
project.events["ReadPlayerLanguage"].params_def="id"
project.events["ReadPlayerLanguage"].params="id"					
project.events["ReadPlayerLanguage"].lastline="return A_PlayerLanguage[id];"

function table.contains_key(list,e)
	for k,_ in pairs(list) do
		if (k==e) then return true end
	end
	return false
end

-- ok, we need to be sure that all light_connections are made properly and
-- only in case that this module is really loaded.
for pos,module in pairs(project.rawmodules) do
	module.depends = {}
	for _,k in pairs(module.depends_light) do
		if (needed_modules[k]) then
			table.insert(module.depends,k)
		end
	end
	project.rawmodules[pos] = module
end

local weights={}
local has_possible_noweights = true
while (has_possible_noweights) do
	local firstnoweight_f = function()
		for pos,module in pairs(project.rawmodules) do
			if (weights[module.name]==nil) then
				local has_unweighted_dependencies = false
				for _,k in pairs(module.depends) do
					if (not table.contains_key(weights,k)) then
						has_unweighted_dependencies = true
					end
				end
				if (not has_unweighted_dependencies) then
					return module
				end
			end
		end
		return nil
	end
	firstnoweight = firstnoweight_f()
	if (firstnoweight==nil) then
		has_possible_noweights=false
	else
		weights[firstnoweight.name]=1
		for _,k in pairs(firstnoweight.depends) do
			weights[firstnoweight.name] = weights[firstnoweight.name] + weights[k]
		end
		
		for pos,module in pairs(project.rawmodules) do
			for _,k in pairs(module.depends) do
				if (k == firstnoweight.name) then
					weights[firstnoweight.name] = weights[firstnoweight.name] + 1
				end
			end
		end
		has_possible_noweights=true
	end
end

if (PRINT_R_WEIGHTS) then
	print_r(weights)
end

function table.isEmpty(t)
	for _,_ in pairs(t) do
		return false 
	end
	return true
end

project.modules={}
while (not table.isEmpty(weights)) do
	local cur_min_val=0
	local cur_min_pos=0
	for pos,val in pairs(weights) do
		if (cur_min_val == 0 or cur_min_val>val) then
			cur_min_val=val
			cur_min_pos=pos
		end
	end
	weights[cur_min_pos]=nil
	for _,mod in pairs(project.rawmodules) do
		if (mod.name==cur_min_pos) then
			table.insert(project.modules,mod)
		end
	end
end 

project["rawmodules"]=nil

-- Get all events from selected modules
for pos,module in pairs(project.modules) do

	-- values
	local values = file_get_contents(".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter..module.name..".values.txt") 
	if (nil ~= values) then
		for line,arrayline in pairs(explode("\n",values)) do
			if (trim(arrayline) ~= "") then
				arrayparts = explode(",",arrayline)
				arrayinfo = {}
				arrayinfo.name=trim(arrayparts[1])
				if (arrayparts[2]==nil) then
					onError("You forgot to add a size parameter for your Value: "..arrayinfo.name,module.name..".values.txt",line)
				end
				arrayinfo.size=trim(arrayparts[2])
				table.insert(module.values,arrayinfo)
				if ( (arrayparts[3] == nil) or (string.find(arrayparts[3],"w")) ) then
					project.events["Write"..arrayinfo.name]={}
					project.events["Write"..arrayinfo.name].has_return=true
					project.events["Write"..arrayinfo.name].name="Write"..arrayinfo.name
					project.events["Write"..arrayinfo.name].module=module.name
					project.events["Write"..arrayinfo.name].params_def="id,newval"
					project.events["Write"..arrayinfo.name].params="id,newval"
					project.events["Write"..arrayinfo.name].lastline="A_"..arrayinfo.name.."[id]=newval;\n	return 1;"
				else
					project.nocallbackdefinitions[arrayinfo.name] = project.nocallbackdefinitions[arrayinfo.name] or {}
					project.nocallbackdefinitions[arrayinfo.name].write = true
				end
				if ( (arrayparts[3] ~= nil) and (string.find(arrayparts[3],"r")) ) then
					project.events["Read"..arrayinfo.name]={}
					project.events["Read"..arrayinfo.name].has_return=true
					project.events["Read"..arrayinfo.name].has_return_text="A_"..arrayinfo.name.."[id]";
					project.events["Read"..arrayinfo.name].name="Read"..arrayinfo.name
					project.events["Read"..arrayinfo.name].module=module.name
					project.events["Read"..arrayinfo.name].params_def="id"
					project.events["Read"..arrayinfo.name].params="id"					
					project.events["Read"..arrayinfo.name].lastline="return A_"..arrayinfo.name.."[id];"
				else
					project.nocallbackdefinitions[arrayinfo.name] = project.nocallbackdefinitions[arrayinfo.name] or {}
					project.nocallbackdefinitions[arrayinfo.name].read = true
				end
			end
		end
	end
	
	-- defines
	local defines = readCfgFile(".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter..module.name..".defines.txt") 
	module.defines = {}
	if (nil ~= defines) then
		local prepared = {}
		local project_mod_cfg = readCfgFile(".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."cfg"..directory_delimiter..module.name..".txt");
		for defkey,defline in pairs(defines) do
			defkey = explode(".",defkey)
			if (defkey[2]==nil) then
				-- its a key, so we use it.
				prepared[defkey[1]]=defline;
				-- reading the cfg line from projects directory now ...
				if (project_mod_cfg[defkey[1]]~=nil) then
					prepared[defkey[1]]=project_mod_cfg[defkey[1]]
				end
			else
				-- its meta info, we ignore it.
			end
		end
		for k,v in pairs(prepared) do
			local item={}
			item.name=k
			item.value=v
			table.insert(module.defines,item)
		end
	end
	
	-- plugins
	local plugins = file_get_contents(".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter..module.name..".plugins.txt") 
	if (nil ~= plugins) then
		for line,pluginname in pairs(explode("\n",plugins)) do
			if (trim(pluginname) ~= "") then
				local dll_fileinfo = lfs.attributes(".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter.."plugins"..directory_delimiter..trim(pluginname)..".dll")
				local so_fileinfo = lfs.attributes(".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter.."plugins"..directory_delimiter..trim(pluginname)..".so")
				if (dll_fileinfo==nil) then
					onError("Can't find "..trim(pluginname)..".dll in "..module.name..directory_delimiter.."plugins-directory.",module.name..".plugins.txt",line)
				end
				local plugininfo = {}
				plugininfo.name=trim(pluginname)
				plugininfo.linux=(so_fileinfo ~= nil)
				plugininfo.windows=(dll_fileinfo ~= nil)
				table.insert(module.plugins,plugininfo)
				if (plugininfo.windows) then
					os.execute(copy_cmd_pre..".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter.."plugins"..directory_delimiter..trim(pluginname)..".dll .."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."build"..directory_delimiter.."plugins"..directory_delimiter..plugininfo.name..".dll >> .."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."debug_output.log",true)
				end
				if (plugininfo.linux) then
					os.execute(copy_cmd_pre..".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter.."plugins"..directory_delimiter..trim(pluginname)..".so .."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."build"..directory_delimiter.."plugins"..directory_delimiter..plugininfo.name..".so >> .."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."debug_output.log",true)
				end
			end
		end
	end
	
	-- exports
	local exports = file_get_contents(".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter..module.name..".exports.txt")
	module.exports={}
	if (exports~=nil) then
		for line,e in pairs(explode("\n",exports)) do
			e = trim(e)
			if (e~="") then
				local has_return = false
				local has_true_return = false
				if (string.sub(e,1,1)==":") then
					has_return = true
					e = string.sub(e,2)
				end
				if (string.sub(e,1,1)=="+") then
					has_true_return = true
					e = string.sub(e,2)
				end
				
				local parts = explode("(",e);
				parts[2] = explode(")",parts[2])[1];
				local func_name = parts[1]
				local params_def = parts[2]
				local raw_params = str_replace("[]","",parts[2])
				raw_params = explode(",",raw_params)
				local params={}
				for _,p in pairs(raw_params) do
					p=explode(":",p)
					if (p[2]==nil) then
						table.insert(params,trim(p[1]));
					else
						table.insert(params,trim(p[2]));
					end
				end
				params=table.concat(params,",")
				
				module.exports[func_name]={}
				module.exports[func_name].has_return=has_return
				if (has_true_return) then
					project.modules[pos].exports[func_name].has_return_text="1"
				end
				module.exports[func_name].name=func_name
				module.exports[func_name].params_def=params_def
				module.exports[func_name].params=params
				
				if (project.events[func_name]~=nil) then
					onError("Two candidates for the event '_"..func_name.."' in "..project.events[func_name].module.." and "..module.name,module.name..".exports.txt",line)
				end
				project.events[func_name]={}
				project.events[func_name].has_return=has_return
				if (has_true_return) then
					project.events[func_name].has_return_text="1"
				end
				project.events[func_name].name=func_name
				project.events[func_name].module=module.name
				project.events[func_name].params_def=params_def
				project.events[func_name].params=params
			end
		end
	end
end

-- Get all languages you want to use
local pos = 0
local default_lang_id = nil
for _,k in pairs(project.cfg.languages) do
	k=trim(k)
	if (k~="") then
		project.languages[pos]=k
		if (k==project.cfg.defaultlanguage) then
			project.defaultlanguageid=pos
		end
		pos = pos + 1
	end
end

-- Get all translations for all used modules
local translationscount=0
local translationelementused={}
for pos,module in pairs(project.modules) do
	for _,lang in pairs(project.languages) do
		local translation = file_get_contents(".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter..module.name.."."..lang..".lng")
		if (translation~=nil) then
			-- Parse the translation content
			for pos,line in pairs(explode("\n",translation)) do 
				local iscomment=string.match(line,"^%s*(#).*")
				line=trim(line)
				if (line=="") then
					-- empty line, ignore it for now.
				elseif (not iscomment) then
					varia,value=string.match(line,"(.*)=(.*)")
					if (varia and string.match(trim(value),"\".*\"")) then 
						-- Init lang translation, if it isn't yet.
						if (project.translations[lang]==nil) then
							project.translations[lang]={}
						end
						project.translations[lang][varia]=value
						translationelementused[varia]=lang
					else
						onWarning("Broken .lng line",module.name.."."..lang..".lng",pos)
					end
				end
			end
		end
	end
end
for _,_ in pairs(translationelementused) do
	translationscount = translationscount + 1
end

local languagecount=0
for pos,val in pairs(project.languages) do
	languagecount = languagecount + 1
end

-- Create new .pwn file
local file_content = {}
if (project.cfg.debug_events==true) then
	table.insert(file_content,"#define DEBUG_EVENTS 1")
end
table.insert(file_content,"#define MAX_STRING 255")
table.insert(file_content,"#define MAX_PLAYERS 500")
table.insert(file_content,"new PL_language[MAX_PLAYERS];")
table.insert(file_content,"#pragma unused PL_language")
table.insert(file_content,"native format(output[], len, const format[], {Float,_}:...);")

-- Create Array macros
table.insert(file_content,"#define SetVal(%1,%2,%3) _Write%1(%2,%3)")
table.insert(file_content,"#define Val(%1,%2) _Read%1(%2)")

-- -- Create Translations
table.insert(file_content,"#define gettext(%1,%2) Translation[PL_language[%1]][T_%2]")
table.insert(file_content,"#define settext(%1,%2,%3)   format(Translation[TL_%1][T_%2],MAX_STRING,\""..directory_delimiter.."37;s\",%3)")
table.insert(file_content,"#define Translation_LanguageCount "..languagecount)
table.insert(file_content,"#define Translation_Texts "..(translationscount+1))
table.insert(file_content,"new LANGUAGENAME[Translation_LanguageCount][MAX_STRING];")
table.insert(file_content,"new Translation[Translation_LanguageCount][Translation_Texts][MAX_STRING];")
table.insert(file_content,"DmoduleInitLangItem(LANG_ID,txt[]) {")

if (translationscount>0) then
	table.insert(file_content,"	for (new i=0;i<Translation_Texts-1; i++) {")
	table.insert(file_content,"		format(Translation[LANG_ID][i],MAX_STRING,\"%s\",Translation["..project.defaultlanguageid.."][i]);")
	table.insert(file_content,"	}")
else
	table.insert(file_content,"#pragma unused Translation")
end

table.insert(file_content,"	format(LANGUAGENAME[LANG_ID],MAX_STRING,\"%s\",txt);")
table.insert(file_content,"}")


table.insert(file_content,"")
local pos = 0
for key,lang in pairs(translationelementused) do
	table.insert(file_content,"#define T_"..key.." "..pos)
	pos = pos + 1
end
table.insert(file_content,"")

table.insert(file_content,"")
for pos,lang in pairs(project.languages) do
	table.insert(file_content,"#define TL_"..string.upper(lang).." "..pos)
end
table.insert(file_content,"")
table.insert(file_content,"DmoduleInitLang() {")

local defaultlangkey = project.languages[project.defaultlanguageid]

-- this is just necessary, if we have any translations
if (translationscount>0) then
	-- first, we'll do the magic for the default language.
	for key,val in pairs(project.translations[defaultlangkey]) do
		table.insert(file_content,[[	settext(]]..string.upper(defaultlangkey)..[[,]]..key..[[,	]]..val..[[);]])
	end
end

for pos,lang in pairs(project.languages) do
	if (pos ~= project.defaultlanguageid) then
		table.insert(file_content,"	DmoduleInitLangItem(TL_"..string.upper(lang)..",\""..lang.."\");")
	end
end

-- this is just necessary, if we have any translations
if (translationscount>0) then
	for langkey,lang in pairs(project.translations) do
		if (langkey ~= defaultlangkey) then
			for key,val in pairs(lang) do
				table.insert(file_content,[[	settext(]]..string.upper(langkey)..[[,]]..key..[[,	]]..val..[[);]])
			end
		end
	end
end

table.insert(file_content,"}")
table.insert(file_content,"")

-- Create the cool PL_array
table.insert(file_content,"new A_PlayerLanguage[MAX_PLAYERS];")
table.insert(file_content,"#pragma unused A_PlayerLanguage")
table.insert(file_content,"#define AS_PlayerLanguage MAX_PLAYERS")


-- -- Create Includes + Array definitions
for pos,module in pairs(project.modules) do
	table.insert(file_content,"#define "..string.upper(module.name).."_LOADED 1")
	for _,defitem in pairs(module.defines) do
		table.insert(file_content,"#define "..defitem.name.." "..defitem.value)
	end
	for _,array in pairs(module.values) do
		table.insert(file_content,"new A_"..array.name.."["..array.size.."];")
		if (project.nocallbackdefinitions[array.name]) then
			if (project.nocallbackdefinitions[array.name].write) then
				table.insert(file_content,"#define _Write"..array.name.."(%1,%2) A_"..array.name.."[%1]=%2");
			end
			if (project.nocallbackdefinitions[array.name].read) then
				table.insert(file_content,"#define _Read"..array.name.."(%1) A_"..array.name.."[%1]");
			end
		end
		table.insert(file_content,"#pragma unused A_"..array.name)
		table.insert(file_content,"#define AS_"..array.name.." "..array.size)
	end
	table.insert(file_content,"#include \".."..directory_delimiter..".."..directory_delimiter..".."..directory_delimiter..".."..directory_delimiter..module.folder..directory_delimiter..module.name..directory_delimiter..module.name..".dmodule.inc\"")
end

table.insert(file_content,"")

table.insert(file_content,"")

-- -- Create Dynamic Loading of projectdir\Event.pwn on event for project
for _,event in pairs(project.events) do
	if (lfs.attributes(".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter..event.name..".pwn") ~= nil) then
		table.insert(file_content,"_pwn_"..event.name.."("..event.params_def..") {")
		for _,param in pairs(explode(",",event.params)) do
			if (param~="") then
				table.insert(file_content,"#pragma unused "..param)
			end
		end
		table.insert(file_content,"#include \".."..directory_delimiter..".."..directory_delimiter..".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter..event.name..".pwn\"")
		table.insert(file_content,"}")
	end
end

-- -- Create Events
for _,event in pairs(project.events) do
	table.insert(file_content,"_"..event.name.."("..event.params_def..") {")
	for _,param in pairs(explode(",",event.params)) do
		if (param~="") then
			table.insert(file_content,"#pragma unused "..param)
		end
	end
	table.insert(file_content,"#if defined DEBUG_EVENTS")
	table.insert(file_content,"	debugprint(\"Call: _"..event.name.."\");")
	table.insert(file_content,"#endif")
	for pos,module in pairs(project.modules) do
		table.insert(file_content,"#if defined _I"..module.name.."_"..event.name)
		table.insert(file_content,"#if defined DEBUG_EVENTS")
		table.insert(file_content,"	debugprint(\"  - Inline:"..module.name.."_"..event.name.."\");")
		table.insert(file_content,"#endif")
		table.insert(file_content,"	_I"..module.name.."_"..event.name)
		table.insert(file_content,"#endif")
		table.insert(file_content,"#if defined "..module.name.."_"..event.name)
		table.insert(file_content,"#if defined _CD_"..module.name.."_"..event.name)
		table.insert(file_content,"	if (_CD_"..module.name.."_"..event.name..") {")
		table.insert(file_content,"#endif")
		table.insert(file_content,"#if defined DEBUG_EVENTS")
		table.insert(file_content,"	debugprint(\"  - "..module.name.."_"..event.name.."\");")
		table.insert(file_content,"#endif")
		if (event.has_return) then
			if (event.has_return_text) then
				table.insert(file_content,"	if ("..module.name.."_"..event.name.."("..event.params..")) return "..event.has_return_text..";")
			else
				table.insert(file_content,"	if ("..module.name.."_"..event.name.."("..event.params..")) return 1;")
			end
		else
			table.insert(file_content,"	"..module.name.."_"..event.name.."("..event.params..");")
		end
		table.insert(file_content,"#if defined _CD_"..module.name.."_"..event.name)
		table.insert(file_content,"	}")
		table.insert(file_content,"#endif")
		table.insert(file_content,"#endif")
	end
	table.insert(file_content,"#if defined DEBUG_EVENTS")
	table.insert(file_content,"	debugprint(\"Done: _"..event.name.."\");")
	table.insert(file_content,"#endif")
	
	if (lfs.attributes(".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter..event.name..".pwn") ~= nil) then
		-- Load the project specific callback file at the end, should be used for initializing project
		-- specific functions
		table.insert(file_content,"	_pwn_"..event.name.."("..event.params..");")
	end
	
	if (event.lastline) then
		table.insert(file_content,"	"..event.lastline)
	else
		if (event.has_return_text) then
			table.insert(file_content,"	return "..event.has_return_text..";")
		else
			if (event.has_return) then
				table.insert(file_content,"	return 0;")
			end
		end
	end
	table.insert(file_content,"}")
	table.insert(file_content,"")
end

file_put_contents(".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."build"..directory_delimiter.."gamemodes"..directory_delimiter..project.name..".pwn",table.concat(file_content,"\n"))

if (project.cfg.server ~= nil) then
	-- the project-cfg variables should be set properly
	for var,key in pairs(project.cfg.server) do
		project.cfg['server.'..var]=key
	end
end

project.cfg['server.plugins']={}
for pos,module in pairs(project.modules) do
	for _,plugin in pairs(module.plugins) do
		table.insert(project.cfg['server.plugins'],plugin.name .. plugins_suffix)
	end
end
project.cfg['server.plugins']=table.concat(project.cfg['server.plugins']," ")

project.cfg['server.gamemode0']=project.name.." 1"
-- Create new server.cfg
local cfg_content={}
table.insert(cfg_content,"# This file is autogenerated by dmodule, please update the file")
table.insert(cfg_content,"#      projects"..directory_delimiter..project.name..directory_delimiter.."project.cfg or settings.cfg")
table.insert(cfg_content,"# to change server configuration for that build")

for var,key in pairs(project.cfg) do
	if (string.find(var,"server.")==1) then
		var = str_replace("server.","",var)
		table.insert(cfg_content,var.." "..key)
	end
end

file_put_contents(".."..directory_delimiter..".."..directory_delimiter.."projects"..directory_delimiter..project.name..directory_delimiter.."build"..directory_delimiter.."server.cfg",table.concat(cfg_content,"\n"))

if (PRINT_R_ALL) then
	print_r(project)
end

