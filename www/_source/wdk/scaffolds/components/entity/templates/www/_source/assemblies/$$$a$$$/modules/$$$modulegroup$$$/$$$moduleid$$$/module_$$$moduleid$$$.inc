<?php

	require_once(GetWDKDir()."wdk_module_entitymanager.inc");
	require_once(GetAssemblyDir("$$$a$$$")."entity_$$$entityname$$$.inc");
	
	class C$$$ModuleID$$$Module extends CEntityManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"$$$modulegroup$$$/$$$moduleid$$$",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$arrayEntityDefinitions = array(
				"$$$entityname$$$" => GetEntityDefinition$$$EntityName$$$()
				);
			$this->SetEntityDefinitions($arrayEntityDefinitions);
			return true;
		}
		
	
		function OnRenderModule($strState)
		{
			$this->Trace("OnRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "":
					/*
						This will define the initial view on the module...
					*/
					return $this->StateList("$$$entityname$$$");
				default:
					return parent::OnRenderModule($strState);
			}
		}
		
		function OnPrepareToolbar()
		{
			parent::OnPrepareToolbar();
		}
				
		function OnCheckInputValues($strEntity,$bAdd,$arrayItem) 
		{
			$arrayErrors = parent::OnCheckInputValues($strEntity,$bAdd,$arrayItem);
			
			if ($strEntity == "$$$entityname$$$")
			{
				/*
				This example demonstrates how specific form input can be checked before
				it is sent to the web service.
				The parent class implementation of this function already covers typical
				input errors. Feel free to override (or extend) error strings that are
				already in arrayErrors[].
				*/
				$fFloatval = floatval($arrayItem["floatval"]);
				if ($fFloatval < 0 || $fFloatval > 1)
				{
					$arrayErrors["floatval"] = "?TID_$$$ENTITYNAME$$$_ERROR_FLOATVAL_WRONGVALUE?";
				}
			}
			return $arrayErrors;
		}
		
		function OnPrepareInputValues($strEntity,$bAdd,$arrayItem) 
		{
			$arrayItem = parent::OnPrepareInputValues($strEntity,$bAdd,$arrayItem);
			
			if ($strEntity == "$$$entityname$$$")
			{
				/*
					This function is called to prepare input entered by a user.
					It should be used to convert "user friendly" input into standardized values.
					E.g. it can be used to convert user friendly date and time formats into ISO format.
				*/
			}
			return $arrayItem;
		}		
				
		function OnPrepareWebserviceParams($strEntity,&$arrayParams)
		{
			if ($strEntity == "$$$entityname$$$")
			{
				/*
				Use this function to modify parameters sent to the
				entity web service.
				
				The example below adds an additional parameter "update_lastaccess"
				to the "get" command.
				
				The web service is implemented in a way that it will update the
				"lastaccess_datetime" attribute only if the "update_lastaccess" parameter
				is given.
				
				You may want to remove this behaviour if you consider views on the entity
				in the manager module an administrative access that should not be counted.
				*/
				if (ArrayGetValue($arrayParams,"command") == "get")
				{
					$arrayParams["update_lastaccess"] = "1";	
				}
			}
		}

			
		function OnPrepareFilterSelectOptions($strEntity,$strFilter)
		{
			/*
				This function is invoked to fill a form's select dropdown
				with values from which the user can choose from for filtering.
				So this is used for the dropdowns in the search options bar in the
				list view.
				
				Keep in mind that if you want the user presented list of options 
				to be alphabetically sorted, you will need to use LoadString() first
				and then sort the contents of the options array as the order might be
				different in different languages.
			*/

			$arrayOptions = array();
			if ($strEntity == "$$$entityname$$$")
			{
				/*
					The example below does exactly the same like the default implementation:
				*/
				if ($strFilter == "filterbytype")
				{
					$arrayOptions = array();
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					$arrayEnum = Get$$$EntityName$$$TypeEnum();
					foreach ($arrayEnum as $strEnum)
					{
						$arrayOptions[$strEnum] = "?TID_$$$ENTITYNAME$$$_TYPE_".StringUpperCase($strEnum)."?";
					}
					return $arrayOptions;
				}
			}
			return parent::OnPrepareFilterSelectOptions($strEntity,$strFilter);
		}


		function OnPrepareSelectOptions($strEntity,$strAttributeID,$strItemIndex)
		{
			/*
				This function is invoked to fill a form's select dropdown
				with values from which the user can choose from when adding or
				modifying an item.
				
				Keep in mind that if you want the user presented list of options 
				to be alphabetically sorted, you will need to use LoadString() first
				and then sort the contents of the options array as the order might be
				different in different languages.
			*/
			
			$arrayOptions = array();
			if ($strEntity == "$$$entityname$$$")
			{
				/*
					In order to use text id for enums there are two ways:
					
					1. The code below in which values to be used in a select
					drop down are replaced with text ids explicitly
				*/
				if ($strAttributeID == "type")
				{
					$arrayOptions = array();
					$arrayEnum = Get$$$EntityName$$$TypeEnum();
					foreach ($arrayEnum as $strEnum)
					{
						$arrayOptions[$strEnum] = "?TID_$$$ENTITYNAME$$$_TYPE_".StringUpperCase($strEnum)."?";
					}
					return $arrayOptions;
				}
				
				/*
					2. By setting the "tidpattern" value within the attribute
					definition. In this case the parent function does the same
					as the code above (but of course in a more generic way).
				*/
				
			}
			return parent::OnPrepareSelectOptions($strEntity,$strAttributeID,$strItemIndex);
		}
		
		
		function OnPrepareItemDataDefaultValues($strEntity,&$arrayItem)
		{
			/*
				This function is called before the form to add a new item
				is populated.
				Modifying the arrayItem overrides defaults defined by the entity
				definition.
				Use this function to implement dynamic defaults.
			*/
			
			parent::OnPrepareItemDataDefaultValues($strEntity,$arrayItem);
			if ($strEntity == "$$$entityname$$$")
			{
				$arrayItem["intval"] = 23;
			}
		}
		
		function OnPrepareItemDataDuplicate($strEntity,&$arrayItem)
		{
			parent::OnPrepareItemDataDuplicate($strEntity,$arrayItem);
			if ($strEntity == "$$$entityname$$$")
			{
				$arrayItem["name"] = ReplaceTagsWDK(
					$this->LoadString("TID_$$$ENTITYNAME$$$_VALUE_COPYOF"),
					array("NAME" => $arrayItem["name"])
					);
			}
		}

		
		function OnItemAdded($strEntity,$strNewItemIndex,$arrayItem)
		{
			/*
				This function is called after a new item has been added by the user.
				The example below sends an email to the website's administrator.
			*/
			$this->GetWebsite()->SendMailToAdmin(
				"A new $$$EntityName$$$ has been added!",
				"The new item's id is $strNewItemIndex.");

			parent::OnItemAdded($strEntity,$strNewItemIndex,$arrayItem);
		}

	
		function OnPrepareItemDataList($strEntity,&$arrayItems,$strSortOption,$strSortOrder)
		{
			/*
				The default implememtation adds task icons
				for sort order modification and calls
				OnPrepareItemDataView() for every icon.
				
				Implement this function to change behaviour only for "list" views.
			*/
			parent::OnPrepareItemDataList($strEntity,$arrayItems,$strSortOption,$strSortOrder);
		}
	
		function OnPrepareItemDataView($strEntity,$strItemIndex,&$arrayItem)
		{
			/*
			Use this function to modify attribute values before being displayed in
			"view" or "list" views.
			*/
			if ($strEntity == "$$$entityname$$$")
			{

				/*
					When modifying the visual appearance of data attributes, keep in mind
					the the parent function already does some default operations
					(like identifying the right text id for enums).
					So in some cases the parent should be called before or after your
					code or not at all.
				*/
			
				parent::OnPrepareItemDataView($strEntity,$strItemIndex,$arrayItem);
				
				$strType = ArrayGetValue($arrayItem,"type");
				if ($strType != "")
				{
					$this->IncludeElement("format");
					$format = new CFormatElement($this->GetWebsite(),"bold");
					$arrayItem["type"] = $format->RenderFormat($strType);
				}
				
			}
			else
			{
				parent::OnPrepareItemDataView($strEntity,$strItemIndex,$arrayItem);
			}
		}
		
		function OnItemView($strEntity,$strItemIndex,$arrayItem)
		{
			/*
				This function is invoked AFTER the view state is prepared.
				The arrayItem contains raw values as received from the web service.
				Use this function to implement customized view states.
				
				The example below reports an event to the website's event management system.
				Depending on it's implementation this may be ignored, logged, or otherwise
				processed.
			*/
			$arrayItem["INDEX"] = $strItemIndex;
			$this->GetWebsite()->SystemEvent("INFORMATIONAL",StringUpperCase($strEntity)."_VIEW",$strItemIndex,$arrayItem);
			
			parent::OnItemView($strEntity,$strItemIndex,$arrayItem);
		}
		
			
 
		function OnPrepareItemDataModify($strEntity,$strItemIndex,&$arrayItem)
		{
			/*
				This function is called BEFORE an item is presented for modification
				to the user. You can modify item attribute values.
				The parent class implementation already covers typical cases, like
				date formatting etc.
			*/
			parent::OnPrepareItemDataModify($strEntity,$strItemIndex,$arrayItem);
		}
		
		function OnItemModified($strEntity,$strItemIndex,$arrayItem)
		{
			/*
				This function is called after an item has been modified by the user.
				The example below sends an email to the website's administrator.
			*/
			$this->GetWebsite()->SendMailToAdmin(
				"$$$EntityName$$$ $strItemIndex has been modified!",
				"");			
			parent::OnItemModified($strEntity,$strItemIndex,$arrayItem);
		}
		
		function OnItemDeleted($strEntity,$strItemIndex,$arrayItem)
		{
			/*
				This function is called after an item has been deleted by the user.
				The example below sends an email to the website's administrator.
			*/
			$this->GetWebsite()->SendMailToAdmin(
				"$$$EntityName$$$ $strItemIndex has been deleted!",
				"");			
			parent::OnItemModified($strEntity,$strItemIndex,$arrayItem);
		}		
		

		function OnPrepareItemDataExport($strEntity,$strItemIndex,$arrayItem)
		{
			/*
				This function is called for each item before being exported
				as a csv file.
				
				The parent implementation takes care for the typical rendering
				contraints. Float values for instance are rendered according to
				the current user's locale preferences - e.g. because a German Excel will
				not interpret the decimal delimiter "." as such.
			*/
			return parent::OnPrepareItemDataExport($strEntity,$strItemIndex,$arrayItem);
		}


	}

