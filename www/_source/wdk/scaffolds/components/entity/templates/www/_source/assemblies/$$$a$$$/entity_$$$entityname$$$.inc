<?php

	require_once(GetWDKDir()."wdk_entitydefinitions.inc");
	
	/*
		This file is the core element of an entity implementation.
		The function GetEntityDefinition$$$EntityName$$$() returns 
		the definition of the $$$EntityName$$$ entity.
		
		The following entity definition demonstrates the capabilities of
		the entity management system.
		The entity definition defines the behaviour of a CDatabaseEntityWebService
		based web service implementation and a GUI module based on CEntityManagerModule.
		An entity manager module implements a full-blown CRUD GUI. CRUD stands for
		"Create, Retrieve, Update, Delete". The GUI can support complex searches,
		multiple column layouts, sorting, paging, adding, viewing, modifying, and
		deleting items.
		
		Check out
		/assemblies/$$$a$$$/$$$modulegroup$$$/$$$moduleid$$$/module_$$$moduleid$$$.inc
		and 
		/assemblies/$$$a$$$/webservices/$$$ws1$$$/$$$ws2$$$/webservice_$$$entityname$$$.inc
		to see how the entity definition interacts with both web service backend
		and the GUI module frontend.
	*/
	
		
	function GetEntityDefinition$$$EntityName$$$()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "$$$entityname$$$_id",
			/*
				The entity definition array defines how the entity is defined and how
				the database entity web service and the entity manager module are
				implemented and how they interact.
			
				"indexattribute" defines the attribute which is used as the unique identifier for an item.
				
				"default"	defines whether this entity should act as the default entity
				within a web service.
				The default entity's web service commands will not be prefixed with the entity name.
				A web service can only support one "default" entity.

				The "indexstate" tells the entity manager module GUI what to do if the item is
				clicked. An alternative to "view" would be "modify".
				
				The "userfriendlyattribute" is used for messages that refer to a
				specific item. An example is the question whether a user really wants
				to delete an item.

				"blocksize" defines the default maximum amount of items being
				displayed in a list.

				"maxblocksize" defines the maximum amount of items returned
				by a web service.

				"dbtablename" specifies the database table name of the entity.

				"noitemsfoundmessage" defines the text message to be displayed
				in the GUI if no items are found based on the selected
				search criteria.
			*/


		
			"default" => true,
			"indexstate" => "view", 
			"userfriendlyattribute" => "name", 
			"blocksize" => 10,
			"maxblocksize" => 100,
			"dbtablename" => "$$$dbtablename$$$",
			"noitemsfoundmessage" => "?TID_$$$ENTITYNAME$$$_INFO_NOITEMSFOUND?",
			"webservice" => array(
				/*
					The "webservice" array contains setting specifc to the entity's
					web service.
				
					The "name" of the web service using the wdk specific
					group/name naming convention.

					"itemtag" defines tag name below the "RESULT" tag that
					contains an item's data.

					When adding an item, "newitemtag" is the tag name that contains the value of the indexattribute
					of the added item.


				*/

			
				"name" => "$$$webservicename$$$",
				"itemtag" => "$$$ENTITYNAME$$$",
				"newitemidtag" => "NEW_$$$ENTITYNAME$$$_ID",
				"commands" => array(
				/*
					The "command" array defines which commands the web service exposes:
					
					If you want to support additional parameters in a generic command,
					you can specify them in the "additionalparams" array. Make sure that
					these parameters are also defined in the web service implementation.
					
				*/
				
					"list" => array(),
					"get" => array(
						"additionalparams" => array("update_lastaccess")),
					"add" => array(),
					"set" => array(),
					"delete" => array()
					)
				)
			,
			"sorting" => array(
			/*
					"defaultattribute" defines by which attribute the items
					are sorted if no selection is made.
					
					The "defaultorder" is either "desc"ending or "asc"ending.
			*/
				"defaultattribute" => "name",
				"defaultorder" => "desc")
			,
			"filteroptions" => array(
			/*
				The filteroptions section defines the search options in the generic GUI.
	
				"submitbuttontext" defines the caption text of the search button.
	
				"reset" defines whether a reset icon link whould be displayed in
				the search options form. The reset link removes all filter
				criteria and will show all available items.

				"reseticon" defines the id of the icon image that should be used for
				the reset icon link.

				"resettooltip" is the tool tip text for the reset icon.

				"formstyle" defines the form's layout style to be used for the
				search form. An alternative to "horizontal" is "vertical".

				"formid": If set a form layout template would be used. This is really
				advanced...
			*/

				"submitbuttontext" => "?TID_$$$ENTITYNAME$$$_BUTTON_FILTER?",
				"reset" => true,
				"reseticon" => "undo",
				"resettooltip" => "?TID_$$$ENTITYNAME$$$_TOOLTIP_RESETFILTERS?",  
				"formstyle" => "horizontal",
				"formid" => "",
				"filters" => array(
				/*
					The filters section defines the available search options:
					
					The name of the filter (e.g. "filterbytype" is your choice.
					
					The "webservicefilteroption" refers to the filter option paramter
					of the web service to be used.

					The "label" defines form control's text label for the filter option.

					Different "type"s of form controls are available for filtering:
					- "select" (a dropdown)
					- "search" (text input)

					If "separator" is set to true, a separator is added to the form.
					The default	layout definition for a "horizontal" form will lead
					to a "line break". Try it!
				*/
				
					"filterbytype" => array(
						"webservicefilteroption" => "type",
						"label" => "?TID_$$$ENTITYNAME$$$_LABEL_FILTERBYTYPE?",
						"type" => "select"
						)
					,
					"searchforname" => array(
						"webservicefilteroption" => "name",
						"label" => "?TID_$$$ENTITYNAME$$$_LABEL_SEARCHFORNAME?",
						"type" => "search"
						)
					,
					"filterbystatus" => array(
						"webservicefilteroption" => "status",
						"label" => "?TID_$$$ENTITYNAME$$$_LABEL_FILTERBYSTATUS?",
						"type" => "select"
						)
					)
				)
			,
			"states" => array(
			/*
				The states define the generic states of the entity manager module GUI.
				You can limit the GUI for the entity by removing some of them.
				E.g. you may not want to enable users to delete items or export data to a csv
				file. Or you may want to go without a view state and offer to modify an item
				directly when being clicked.
			*/
				"list",
				"add",
				"delete",
				"view",
				"modify",
				"modifysortorder",
				"duplicate",
				"export",
				"import"
				)
			,
			
			"tasks" => array(
			/*
				"Tasks" are GUI options associated to each item. They are displayed in the list
				view in each item's row or if one item is selected they are displayed in the tool bar.
				
				"state" defines the module state to be invoked when the icon is clicked.
				"icon" defines the icon image id to be used for the task icon.
				"tooltip" can be used to define a tooltip text that is displayed in a pop up window
				when the user hovers the mouse of the icon. If it is not set, a TID_ name will be derived
				automatically.
				
			*/
				"view" => array(
					"state" => "view",
					"icon" => "view"
				),
				"modify" => array(
					"state" => "modify",
					"icon" => "modify"
				),
				"duplicate" => array(
					"state" => "duplicate",
					"icon" => "duplicate"
				),
				"delete" => array(
					"state" => "delete",
					"icon" => "delete"
				)
			)
			,
			
			
			"view" => array(
			/*
				The "view" section defines the behaviour of the view state (if it exists).
				
				"style" is one out of "vertical", "horizontal",	"custom", or "text".
				Default is "vertical".
				
				"hideheader" does not use the friendly name as a header but simply shows
				the item name in the same way as all other attributes.
				
				"refresh" defines if a refresh link should be shown in the view
				state. Should be set to false for entities which are not likely to
				change often and true for items that are likely to change
				(like sessions or data items).
			*/			
				"refresh" => false,
				"style" => "vertical",
				"hideempty" => false,
				"hideheader" => false,
				"headerlevel" => 0,
				"headerstyle" => ""
			),
			
			
			"list" => array(
			/*
				The "list" section defines the behaviour of the list state.
				
				"refresh" defines if a refresh link should be shown in the list
				state. Should be set to false for entities which are not likely to
				change often and true for items that are likely to change
				(like sessions or data items).
			*/			
				"refresh" => false
			),			
			
			"tabs" => array(
			/*
				If there are too many columns to be displayed in the list view, you can group
				them. A tab control is used to navigate between these column groups.
				The "tab" name can be referenced in the attribute definition.
				The value of the "tab" entry defines the text caption to be used in the "tab".
			*/
			
				"details" => "?TID_$$$ENTITYNAME$$$_TAB_DETAILS?",
				"history" => "?TID_$$$ENTITYNAME$$$_TAB_HISTORY?"
				)
			,
			"attributes" => array( 
			/*
				The attributes array defines the actual database table and
				it's representation via the web service and the GUI.
				
				"webserviceparam",
				"exportcolumnheader",
				"sortoption",
				"filteroption",
				"sharecolumn",
				"type",
				"dbtype",
				"control",
				"default",
				"maxlen",
				"minlen",
				"max",
				"min",
				"floatprecision",
				"zerounset", 
				"inputrequired", 
				"charset",
				"enum",
				"tidpattern",
				"tableheader",
				"formlabel",
				"readonly",
				"readonlyfrontend",
				"excludefromget",
				"excludefromlist",
				"hideinlist",
				"hideinview",
				"hideinexport",
				"nostorage",
				"indexlink",
				"tab"
				
			*/
				"sortorder" => array(
					"webserviceparam" => true,
					"type" => "sortorder",
					"sortoption" => true,
					"sharecolumn" => false
					)
				,
				"$$$entityname$$$_id" => array(
					"webserviceparam" => true,
					"type" => "numericid",
					"sortoption" => true,
					"readonly" => true,
					"indexlink" => true,
					"sharecolumn" => false
					)
				,
				"name" => array(
					"webserviceparam" => true,
					"type" => "string", 
					"minlen" => 1,
					"maxlen" => 100,
					"sortoption" => true,
					"filteroption" => true,
					"indexlink" => true,
					"sharecolumn" => false
					)
				,
				"type" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"enum" => Get$$$EntityName$$$TypeEnum(),
					"default" => "TYPE1",
					"type" => "enum",
					"sharecolumn" => false
					)
				,					
				"status" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"enum" => Get$$$EntityName$$$StatusEnum(),
					"tidpattern" => "?TID_$$$ENTITYNAME$$$_STATUS_{VALUE}?",
					"default" => "STATUS1",
					"type" => "enum",
					"sharecolumn" => true
					)
				,									
				"active" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"type" => "bool",
					"sharecolumn" => false
					)
				,					
				"intval" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "integer",
					"min" => "0",
					"max" => "256",
					"tab" => "details",
					"sharecolumn" => true
					)
				,					
				"floatval" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "float",
					"min" => "0",
					"max" => "1",
					"floatprecision" => "3",
					"tab" => "details"
					)
				,					
				"creation_datetime" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "datetime",
					"tab" => "history",
					"sharecolumn" => true)
				,
				"lastchange_datetime" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "datetime",
					"tab" => "history",
					"sharecolumn" => true)
				,
				"lastaccess_datetime" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "datetime",
					"tab" => "history",
					"sharecolumn" => false)
				,
				"content" => array(
					"webserviceparam" => true,
					"readonly" => false,
					"filteroption" => true,
					"minlen" => 0,
					"maxlen" => 2000,
					"type" => "string",
					"hideinlist" => true,
					"hideinview" => true,
					"control" => "textarea")
			)		
		);
		
		return $arrayEntityDefinition;
	}

	function Get$$$EntityName$$$TypeEnum()
	{
		$arrayEnum = array(
			0 => "TYPE1",
			1 => "TYPE2",
			2 => "TYPE3"
			);
		return $arrayEnum;
	}

	function Get$$$EntityName$$$StatusEnum()
	{
		$arrayEnum = array(
			0 => "STATUS1",
			1 => "STATUS2",
			2 => "STATUS3"
			);
		return $arrayEnum;
	}

	
