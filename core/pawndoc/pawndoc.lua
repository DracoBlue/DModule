-- Copyright 2007 by Jan Schuetze (DracoBlue), DracoBlue.net
-- http://dracoblue.net/pawndoc
-- PawnDoc 0.1
-- See Copyright Notice in LICENSE.txt
function htmlspecialchars(source)
	local entities = {
	['"'] = '&quot;' ,
	["'"] = '&#39;' ,
	['<'] = '&lt;' ,
	['>'] = '&gt;' ,
	['&'] = '&amp;'
	}
	local str = string.gsub(source, "[^a-zA-Z0-9 _]",
		function (v)
			if entities[v] then return entities[v] else return v end
		end)
	return str
end

if (arg[1]==nil or arg[2]==nil) then
	print("Correct usage: pawndoc sourcefile.pwn html\"targetfile.html\"")
	os.execute("pause")
	os.exit()
end

TO_WIKI=false
TO_HTML=false
towikifiles={}
tohtmlfiles={}
for id,a in pairs(arg) do
	if (id == 1) then
		-- the name of the sourcefile
		filename = a
	end		
	if (id>1) then
		is_valid_engine = false
		is_wiki, fname = string.match(a,"^(wiki)(.*)$")
		if (is_wiki) then
			TO_WIKI=true
			table.insert(towikifiles,fname)
			is_valid_engine = true
		end
		is_html, fname = string.match(a,"^(html)(.*)$")
		if (is_html) then
			TO_HTML=true
			table.insert(tohtmlfiles,fname)
			is_valid_engine = true
		end
		if (not is_valid_engine) then
			-- unknown engine
			print("Unknown engine. Use html\"targetfile.html\" to activate the html-engine.")
			os.exit()
		end
	end
end

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

function trim(s)
	return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

function str_replace(old,new,str)
	return table.concat(explode(old,str),new)
end

local file = io.open(filename, "r")
if (file) then
	file:close()
else
	-- we can't find that source file
	print("Can't find source file: "..filename)
	os.exit()
end

fileContent = explode("\n",file_get_contents(filename))

docelements={}

local current_element={}
local ANYWHERE					= 0
local IN_DOC					= 1
local NEXT_LINE_IS_SIGNATURE	= 2
local cur_state = ANYWHERE
local current_item=nil
for _,line in pairs(fileContent) do
	if (cur_state == NEXT_LINE_IS_SIGNATURE) then
		cur_state = ANYWHERE
		is_useful_signature = false
		is_stock, function_name, function_params = string.match(line,"%s*(stock)%s+([%w_]+)%s*%(([^%)]*)%)")
		if (function_params==nil) then
			function_params=""
		end
		if (is_stock) then
			current_element.signature = function_name .. "(" .. function_params .. ")"
			current_element.function_type = "stock"
			current_element.function_name = function_name
			current_element.function_params = function_params
			is_useful_signature = true
		end
		is_public, function_name, function_params = string.match(line,"%s*(public)%s+([%w_]+)%s*%(([^%)]*)%)")
		if (is_useful_signature==false and is_public) then
			current_element.signature = function_name .. "(" .. function_params .. ")"
			current_element.function_type = "public"
			current_element.function_name = function_name
			current_element.function_params = function_params
			is_useful_signature = true
		end
		is_new, variable_name, variable_extra = string.match(line,"%s*(new)%s+([^%;%[]+)%s*([^%;]*)%;")
		if (is_useful_signature==false and is_new) then
			current_element.signature = variable_name .. variable_extra
			current_element.variable_name = variable_name
			current_element.variable_extra = variable_extra
			is_useful_signature = true
		end
		function_name, function_params = string.match(line,"%s*([%w_]+)%s*%(([^%)]*)%)")
		if (is_useful_signature==false and function_name) then
			current_element.signature = function_name .. "(" .. function_params .. ")"
			current_element.function_type = "normal"
			current_element.function_name = function_name
			current_element.function_params = function_params
			is_useful_signature = true
		end
		if (is_useful_signature) then
			table.insert(docelements,current_element)
		end
		current_element={}
		current_item=nil
	else
		if (cur_state == ANYWHERE) then
			--detect type of line
			is_doc_start, doc_start_content = string.match(line,"%s*(%/)%*%*(.*)")
			if (is_doc_start) then
				cur_state = IN_DOC
				current_element.doc_content={}
				current_element.description={}
				current_element.param={}
				current_element.ret={}
				current_element.author=""
				current_element.date=""
				current_element.update=""
				current_element.groups={}
				current_element.see={}
				current_element.deprecated={}
				current_element.deprecated.is=false
				current_element.deprecated.description={}
				current_element.ret.description={}
				if (trim(doc_start_content)~="") then
					table.insert(current_element.doc_content,trim(doc_start_content))
				end
			else
				-- its no start .. lets ignore it ...
			end
		else
			if (cur_state == IN_DOC) then
				is_doc_end = string.match(line,"%s*%*(%/)")
				if (is_doc_end) then
					-- next step is singature
					cur_state = NEXT_LINE_IS_SIGNATURE
				else
					-- lets append it
					is_valid_line, linecontent = string.match(line,"%s*(%*)%s*(.*)")
					if (is_valid_line) then
						linecontent = trim(linecontent)
						table.insert(current_element.doc_content,linecontent)
						if (string.sub(linecontent,1,1)=="@") then
							-- its a key, so new item
							attribut_linetype, attributecontent = string.match(linecontent,"%@([%w_]+)%s*(.*)")
							if (attribut_linetype==nil) then
								-- TODO: Add warning or something like that
							else
								attribut_linetype = string.lower(attribut_linetype)
								was_valid_attribute = false
								current_item = {}
								if (attribut_linetype=="param") then
									was_valid_attribute = true
									param_type, param_name, param_descr = string.match(attributecontent,"([%w_]+)%s*([%w_]+)%s*(.*)")
									current_item.type="param"
									current_item.name=param_name
									current_element.param[param_name]={};
									current_element.param[param_name].description={}
									current_element.param[param_name].type=param_type
									if (param_descr~=nil) then
										table.insert(current_element.param[param_name].description,param_descr)
									end
								end
								if (attribut_linetype=="return") then
									ret_type, ret_descr = string.match(attributecontent,"([%w_]+)%s*(.*)")
									current_item.type="return"
									if (ret_descr~=nil) then
										table.insert(current_element.ret.description,ret_descr)
									end
									current_element.ret.type=ret_type
									was_valid_attribute = true
								end
								if (attribut_linetype=="author") then
									current_element.author = attributecontent
									if (current_element.author~=nil) then current_element.author=trim(current_element.author) end
									was_valid_attribute = true
									-- even though it was a valid one, we can't continue that
									current_item=nil
								end
								if (attribut_linetype=="date") then
									current_element.date = attributecontent
									if (current_element.date~=nil) then current_element.date=trim(current_element.date) end
									was_valid_attribute = true
									-- even though it was a valid one, we can't continue that
									current_item=nil
								end
								if (attribut_linetype=="update") then
									current_element.update = attributecontent
									if (current_element.update~=nil) then current_element.update=trim(current_element.update) end
									was_valid_attribute = true
									-- even though it was a valid one, we can't continue that
									current_item=nil
								end
								if (attribut_linetype=="deprecated") then
									current_item.type="deprecated"
									current_element.deprecated.is=true
									deprecated_info = attributecontent
									if (deprecated_info~=nil) then
										table.insert(current_element.deprecated.description,trim(deprecated_info))
									end
									was_valid_attribute = true
								end
								if (attribut_linetype=="ingroup") then
									current_item.type="ingroup"
									groupname = attributecontent
									if (groupname~=nil) then
										if (trim(groupname)~="") then
											table.insert(current_element.groups,trim(groupname))
										end
									end
									was_valid_attribute = true
								end
								if (attribut_linetype=="see") then
									see_item, see_description = string.match(attributecontent,"([%w_]+)%s*(.*)")
									if (see_item~=nil) then
										if (trim(see_item)~="") then
											local t = {}
											t.name=trim(see_item)
											t.description=trim(see_description)
											table.insert(current_element.see,t)
										end
									end
									was_valid_attribute = true
									-- even though it was a valid one, we can't continue that
									current_item=nil
								end
								if (not was_valid_attribute) then
									-- TODO: Add warning or something like that
									current_item = nil
								end
							end
						else
							-- its not a key, so just content for current item
							if (current_item==nil) then
								-- its the description of our current_element
								table.insert(current_element.description,linecontent)
							else
								-- its the description of an item, like param/return or ingroup
								if (current_item.type=="param") then
									table.insert(current_element.param[current_item.name].description,linecontent)
								end
								if (current_item.type=="return") then
									table.insert(current_element.ret.description,linecontent)
								end
								if (current_item.type=="deprecated") then
									table.insert(current_element.deprecated.description,linecontent)
								end
								if (current_item.type=="ingroup") then
									if (trim(linecontent)~="") then
										table.insert(current_element.groups,trim(linecontent))
									end
								end
							end
						end
					else
						-- TODO: Add warning or something like that
					end
				end
			end
		end
	end
end

-- lets do some recontructing, order by group
docgroups = {}

local cur_has_a_group = false
for _,element in pairs(docelements) do
	cur_has_a_group = false
	for _,group in pairs(element.groups) do
		cur_has_a_group = true
		if (docgroups[group]==nil) then
			docgroups[group]={}
		end
		table.insert(docgroups[group],element)
	end
	if (cur_has_a_group==false) then
		if (docgroups[""]==nil) then
			docgroups[""]={}
		end
		table.insert(docgroups[""],element)
	end
end

-- no lets do the output

if (TO_WIKI) then
	filecontent_wiki={}
end

if (TO_HTML) then
	filecontent_html={}
	html_ids_used={}
title = filename
table.insert(filecontent_html,[[<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
	<title>]]..htmlspecialchars(title)..[[</title>
	<style type="text/css">
	
#pageMadeWith {
	height:33px;
	width:88px;
	position:absolute;
	right:22px;
	top:22px;
}

div#pageMadeWith img {
	border:0px;
}

body {
	padding:5px;
	margin:5px;
}

a {
color:#44F;
}

h1 {
	background-color:#EAEAEA;
	padding:10px;
	margin:0 0 20px 0;
	font-size:32px;
}

h2.group {
padding:0px;
margin:0 0 10px 10px;
border-bottom:1px solid #000;
}

div.group_content {
margin-left:30px;
}

ul.parameters {
margin:0px;
padding:0 0 0 30px;
list-style-type:none;
font-size:90%;
}

h3.function, h3.procedure {
padding:0px;
margin:0px;
}

div.description {
  margin-left:20px;
}

div.see_links {
font-size:70%;
margin-left:20px;
margin-top:5px;
}

div.procedure_content {
margin-bottom:20px;
}

#pageFooter {
  padding:10px;
  background-color:#EAEAEA;
  text-align:center;
}

#pageFooter img {
  height:31px;
  width:88px;
  border:0px;
}

	</style>
</head>
<body><div id="page"><div id="page2">]])
table.insert(filecontent_html,"<h1>"..htmlspecialchars(title).."</h1>")
	
end

if (TO_WIKI or TO_HTML) then
	for group,elements in pairs(docgroups) do
		group = string.upper(string.sub(group,1,1))..string.sub(group,2)
		if (group=="") then
			if (TO_WIKI) then
				table.insert(filecontent_wiki,"== (no group) ==")
			end
			if (TO_HTML) then
				table.insert(filecontent_html,"<h2 class=\"group\">(no group)</h2>")
			end
		else
			if (TO_WIKI) then
				table.insert(filecontent_wiki,"=="..group.."==")
			end
			if (TO_HTML) then
				table.insert(filecontent_html,"<h2 class=\"group\">"..htmlspecialchars(group).."</h2>")
			end
		end
		if (TO_HTML) then
			table.insert(filecontent_html,"<div class=\"group_content\">")
		end
		
		sorted_elements={}
		local list_of_element_names = {}
		for _,element in pairs(elements) do
			table.insert(list_of_element_names,element.function_name)
		end
		table.sort(list_of_element_names)
		for _,element_name in pairs(list_of_element_names) do
			for _,element in pairs(elements) do
				if (element.function_name==element_name) then
					table.insert(sorted_elements,element)
				end
			end
		end
		
		for _,element in pairs(sorted_elements) do
			if (element.function_name~=nil) then
				if (TO_HTML) then
					table.insert(filecontent_html,"<div class=\"procedure_content\">")
				end
				if (TO_HTML) then
					html_id_extra = ""
					if (html_ids_used[element.function_name]==nil) then
						html_ids_used[element.function_name]=0
						html_id_extra = "id=\""..htmlspecialchars(element.function_name).."\" "
					end
					html_ids_used[element.function_name] = html_ids_used[element.function_name] + 1
				end
				if (element.ret.type==nil) then
					if (TO_WIKI) then
						table.insert(filecontent_wiki,"{{PWNProcedureName|"..element.function_name.."|"..str_replace("=","<nowiki>=</nowiki>",element.function_params).."}}")
					end
					if (TO_HTML) then
						table.insert(filecontent_html,"<h3 "..html_id_extra.."class=\"procedure\">"..htmlspecialchars(element.function_name).."("..htmlspecialchars(element.function_params)..")</h3>")
					end
				else
					if (TO_WIKI) then
						table.insert(filecontent_wiki,"{{PWNFunctionName|"..element.function_name.."|"..str_replace("=","<nowiki>=</nowiki>",element.function_params).."|"..element.ret.type.."}}")
					end
					if (TO_HTML) then
						table.insert(filecontent_html,"<h3 "..html_id_extra.."class=\"function\">"..htmlspecialchars(element.function_name).."("..htmlspecialchars(element.function_params)..") : "..element.ret.type.."</h3>")
					end
				end
				
				param_count = 0
				for param_name,param in pairs(element.param) do
					param_count = param_count + 1
				end
				
				if (TO_HTML) then
					if (param_count>0) then
						table.insert(filecontent_html,"<ul class=\"parameters\">")
					end
				end
				
				for param_name,param in pairs(element.param) do
					if (TO_WIKI) then
						table.insert(filecontent_wiki,"{{PWNParameter|"..param_name.."|"..table.concat(param.description,"\n").."|"..param.type.."}}")
					end
					if (TO_HTML) then
						table.insert(filecontent_html,"	<li class=\"parameter\">"..htmlspecialchars(param_name).." : "..htmlspecialchars(param.type).."<br/><i>"..htmlspecialchars(table.concat(param.description,"\n")).."</i></li>")
					end
				end
				if (TO_HTML) then
					if (param_count>0) then
						table.insert(filecontent_html,"</ul>")
					end
				end
				if (TO_HTML) then
					table.insert(filecontent_html,"<div class=\"description\">"..htmlspecialchars(table.concat(element.description,"\n")).."</div>")
				end
				if (TO_HTML) then
					has_see_tags = false
					for _,_ in pairs(element.see) do
						has_see_tags = true
					end
					if (has_see_tags) then
						is_first = true
						table.insert(filecontent_html,"<div class=\"see_links\">See: ")
						for _,see in pairs(element.see) do
							if (not is_first) then
								table.insert(filecontent_html,",")
							else
								is_first = false
							end
							table.insert(filecontent_html,"<a href=\"#"..htmlspecialchars(see.name).."\">"..htmlspecialchars(see.name).."</a>")
						end
						table.insert(filecontent_html,"</div>")
					end
					table.insert(filecontent_html,"</div>")
				end
				if (TO_WIKI) then
					table.insert(filecontent_wiki,"{{PWNDescription|<nowiki>"..table.concat(element.description,"\n").."</nowiki>}}")
					table.insert(filecontent_wiki,"")
					table.insert(filecontent_wiki,"")
				end
			end
		end
		if (TO_HTML) then
			table.insert(filecontent_html,"</div>")
		end
		if (TO_WIKI) then
			table.insert(filecontent_wiki,"")
			table.insert(filecontent_wiki,"")
		end
	end
end

if (TO_WIKI) then
	for _,fname in pairs(towikifiles) do
		file_put_contents(fname,table.concat(filecontent_wiki,"\n"))
	end
end

if (TO_HTML) then
	table.insert(filecontent_html,"</div></div><div id=\"pageFooter\">")
	table.insert(filecontent_html,[[<a href="http://dracoblue.net/pawndoc"><img src="http://dracoblue.net/pawndoc/pawndoc-10.png" alt="Generated with PawnDoc 1.0" height="31" width="88" /></a>]])
	table.insert(filecontent_html,[[<a href="http://validator.w3.org/check?uri=referer"><img src="http://www.w3.org/Icons/valid-xhtml10-blue" alt="Valid XHTML 1.0 Strict" height="31" width="88" /></a>]])
	table.insert(filecontent_html,[[<a href="http://jigsaw.w3.org/css-validator/check/referer"><img style="border:0;width:88px;height:31px" src="http://jigsaw.w3.org/css-validator/images/mwcss" alt="Valid CSS!" /> </a>]])
	table.insert(filecontent_html,"</div>")
	table.insert(filecontent_html,"<div id=\"pageMadeWith\">")
	table.insert(filecontent_html,[[<a href="http://dracoblue.net/pawndoc"><img src="http://dracoblue.net/pawndoc/pawndoc-10.png" alt="Generated with PawnDoc 1.0" height="31" width="88" /></a>]])
	table.insert(filecontent_html,"</div></body>")
	table.insert(filecontent_html,"</html>")
	for _,fname in pairs(tohtmlfiles) do
		file_put_contents(fname,table.concat(filecontent_html,"\n"))
	end
end