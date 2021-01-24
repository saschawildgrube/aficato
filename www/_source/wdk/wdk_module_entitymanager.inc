<?php

	require_once("wdk_entitydefinitions.inc");
	require_once("wdk_entity.inc");
	require_once("wdk_httpupload.inc");
	
	abstract class CEntityManagerModule extends CModule
	{
		private $m_entityDef;
		private $m_toolbarDef;
		private $m_arrayErrors;
		private $m_bErrorDeleteConfirm;
		private $m_linkicon;
		private $m_linkiconButton;
		private $m_link;
		
		function __construct(
			CWebSite $website, 
			$strModuleID,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			$this->m_arrayErrors = array();
			$this->m_toolbarDef = new CToolbarDefinition();
			$this->m_entityDef = new CEntityDefinitions();
			$this->m_bErrorDeleteConfirm = false;
			$this->m_linkicon = null;
			$this->m_linkiconButton = null;
			$this->m_link = null;
			$this->m_strExportDecimalDelimiter = ".";
			$website->IncludeElement("itemmanager");
			$website->IncludeElement("collection");
			parent::__construct($website, $strModuleID, $strInstanceID, $arrayConfig);
		}


		function OnInit()
		{
			$bResult = parent::OnInit();
			return $bResult;
		}

		final		
		function SetEntityDefinitions($arrayEntityDefinitions)
		{
			$this->Trace("SetEntityDefinitions");
			//$this->Trace($arrayEntityDefinitions);
			$arrayErrors = array();
			$this->m_entityDef->SetEntityDefinitions($arrayEntityDefinitions);
			$bResult = $this->m_entityDef->CheckEntityDefinitions($arrayErrors,ENTITYDEF_MODULE);
			if ($bResult == false)
			{
				$this->Trace("Entity Definition Check failed!");
				$this->Trace($arrayErrors);
				$this->Error("Entity Definition Check failed");
				return $bResult;
			}
			foreach ($arrayEntityDefinitions as $strEntity => $arrayEntityDefinition)
			{
				$this->GetWebsite()->LoadStringResources("entity-".$strEntity);
			}
			return true;
		}

		final
		function AssertValidEntity($strEntity)
		{
			if ($this->m_entityDef->AssertValidEntity($strEntity) == false)
			{
				$this->Error("AssertValidEntity(\"$strEntity\") failed!");	
			}
		}
	
		final
		function GetEntityDefinitions()
		{
			return $this->m_entityDef->GetEntityDefinitions();
		}
	
		final
		function GetEntityDefinition($strEntity)
		{
			$arrayEntityDefinition = $this->m_entityDef->GetEntityDefinition($strEntity);
			if (!is_array($arrayEntityDefinition))
			{
				$this->Trace("GetEntityDefinition(\"$strEntity\"): Entity is not defined.");
				return false;	
			}
			return $arrayEntityDefinition;
		}

		final
		function GetEntityAttributesDefinition($strEntity)
		{
			return ArrayGetValue(
				$this->GetEntityDefinition($strEntity),
				"attributes");
		}

		
		final
		function GetEntityAttributeDefinition($strEntity,$strAttributeID)
		{
			return ArrayGetValue(
				$this->GetEntityAttributesDefinition($strEntity),
				$strAttributeID);
		}
		
		final
		function IsReadOnly()
		{
			return GetBoolValue($this->GetModuleConfig("readonly"));
		}
		
		final
		function GetContextFilters($strEntity)
		{
			$arrayContextFilters = $this->GetModuleConfig($strEntity,"contextfilters");
			if (!is_array($arrayContextFilters))
			{
				$arrayContextFilters = array();	
			}	
			return $arrayContextFilters;
		}
		
		final
		function CheckCondition($strEntity,$strContext)
		{
			$arrayEntityDefinition = $this->m_entityDef->GetEntityDefinition($strEntity);
			$strCondition = ArrayGetValue($arrayEntityDefinition,"conditions",$strContext);
			if ($strCondition == "")
			{
				return true;	
			}
			$bResult = $this->GetWebsite()->CheckCondition($strCondition);
			if ($bResult == false)
			{
				$this->Trace("CheckCondition(\"$strEntity\",\"$strContext\") returns false");				
			}
			return $bResult;
		}
		final
		function CheckConditionForItem($strEntity,$arrayItem,$strState)
		{
			return $this->OnCheckConditionForItem($strEntity,$arrayItem,$strState);
		}
		
		function OnCheckConditionForItem($strEntity,$arrayItem,$strContext)
		{
			return "";	
		}	

		final
		function GetItemIndexPropertyModuleVarName($strEntity)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			if ($arrayEntity == false)
			{
				$this->Error("Entity \"$strEntity\" is not configured.");
				return "error";	
			} 
			
			$strIndexProperty = ArrayGetValue($arrayEntity,"indexattribute");
			if ($strIndexProperty == "")
			{
				$this->Error("No \"indexattribute\" defined for entity \"$strEntity\".");
				return "error";	
				
			}
			return $this->GetModuleVarName($strIndexProperty);
		}
		final
		function GetItemIndexPropertyModuleVar($strEntity)
		{
			return $this->GetVar(
				$this->GetItemIndexPropertyModuleVarName($strEntity));
		}
		final
		function GetItemIndexPropertyWebserviceParamName($strEntity)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			if ($arrayEntity == false)
			{
				$this->Error("Item \"$strEntity\" is not configured.");
				return "error";	
			} 
			
			$strIndexProperty = ArrayGetValue($arrayEntity,"indexattribute");
			return $strIndexProperty;
		}
		
		final
		function IsStateSupported($strEntity,$strState)
		{
			$arrayStates = ArrayGetValue(
					$this->GetEntityDefinition($strEntity),
					"states");
			if (!is_array($arrayStates))
			{
				return false;	
			}
			if ($this->CheckCondition($strEntity,$strState) == false)
			{
				return false;		
			}
			if ($this->IsReadOnly())
			{
				if ($strState == "add")
				{
					return false;	
				}
				if ($strState == "delete")
				{
					return false;	
				}
				if ($strState == "modify")
				{
					return false;	
				}
				if ($strState == "modifysortorder")
				{
					return false;	
				}
				if ($strState == "duplicate")
				{
					return false;	
				}
				if ($strState == "import")
				{
					return false;	
				}
			}
			$arrayContextFilters = $this->GetContextFilters($strEntity);
			if (ArrayCount($arrayContextFilters) > 0)
			{
				if ($strState == "import")
				{
					return false;	
				}
				if ($strState == "export")
				{
					return false;	
				}
			}
			return ArrayValueExists($arrayStates,$strState);
		}
		
		final
		function IsSortOrderTaskVisible($strEntity,$strAttribute)
		{
			if ($this->IsAttributeReadOnly($strEntity,$strAttribute) == true)
			{
				return false;	
			}
			if ($this->IsReadOnly() == true)
			{
				return false;	
			}
			if ($this->IsStateSupported($strEntity,"modifysortorder") == false)
			{
				return false;
			}
			return true;
		}
		
		final
		function GetItemWebserviceCommand($strEntity,$strCommandName)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$strCommand = ArrayGetValue($arrayEntity,"webservice","commands",$strCommandName,"name");
			if ($strCommand == "")
			{
				$bDefault = GetBoolValue(ArrayGetValue($arrayEntity,"default"));
				if ($strCommandName == "list")
				{
					$strCommand = ($bDefault)?("list"):($strEntity."list");	
				}
				else
				{
					$strCommand = ($bDefault)?($strCommandName):($strCommandName.$strEntity);	
				}
			}
			return $strCommand;
		}

		
		function OnRenderModule($strState)
		{
			$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\")");
			$arrayEntityDefinitions = $this->m_entityDef->GetEntityDefinitions();
			foreach ($arrayEntityDefinitions as $strEntity => $arrayEntity)
			{
				if ($strState == $strEntity."_list")
				{
					if ($this->CheckCondition($strEntity,"list"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateList(\"$strEntity\")");
						return $this->StateList($strEntity);
					}
			
				}
				elseif ($strState == $strEntity."_view")
				{
					if ($this->IsStateSupported($strEntity,"view"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateView(\"$strEntity\")");
						return $this->StateView($strEntity);
					}
					else
					{
						return $this->OnRenderModule($strEntity."_list");
					}
				}
				elseif ($strState == $strEntity."_add")
				{
					if ($this->IsStateSupported($strEntity,"add"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateAdd(\"$strEntity\")");
						return $this->StateAdd($strEntity);
					}
					else
					{
						return $this->OnRenderModule($strEntity."_list");
					}
				}
				elseif ($strState == $strEntity."_addsubmit")
				{
					if ($this->IsStateSupported($strEntity,"add"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateAddSubmit(\"$strEntity\")");
						return $this->StateAddSubmit($strEntity);
					}				
					else
					{
						return $this->OnRenderModule($strEntity."_list");
					}
				}
				elseif ($strState == $strEntity."_modify")
				{
					if ($this->IsStateSupported($strEntity,"modify"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateModify(\"$strEntity\")");
						return $this->StateModify($strEntity);
					}				
					else
					{
						return $this->OnRenderModule($strEntity."_view");
					}
				}
				elseif ($strState == $strEntity."_modifysubmit")
				{
					if ($this->IsStateSupported($strEntity,"modify"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateModifySubmit(\"$strEntity\")");
						return $this->StateModifySubmit($strEntity);
					}				
					else
					{
						return $this->OnRenderModule($strEntity."_view");
					}
				}
				elseif ($strState == $strEntity."_modifysortorder")
				{
					if ($this->IsStateSupported($strEntity,"modifysortorder"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateModifySortOrder(\"$strEntity\")");
						return $this->StateModifySortOrder($strEntity);
					}				
					else
					{
						return $this->OnRenderModule($strEntity."_list");
					}
				}
				elseif ($strState == $strEntity."_duplicate")
				{
					if ($this->IsStateSupported($strEntity,"duplicate"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateDuplicate(\"$strEntity\")");
						return $this->StateDuplicate($strEntity);
					}				
					else
					{
						return $this->OnRenderModule($strEntity."_view");
					}
				}
				elseif ($strState == $strEntity."_duplicatesubmit")
				{
					if ($this->IsStateSupported($strEntity,"duplicate"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateDuplicateSubmit(\"$strEntity\")");
						return $this->StateDuplicateSubmit($strEntity);
					}				
					else
					{
						return $this->OnRenderModule($strEntity."_duplicate");
					}
				}
				elseif ($strState == $strEntity."_delete")
				{
					if ($this->IsStateSupported($strEntity,"delete"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateDelete(\"$strEntity\")");
						return $this->StateDelete($strEntity);
					}				
					else
					{
						return $this->OnRenderModule($strEntity."_view");
					}
				}
				elseif ($strState == $strEntity."_deletesubmit")
				{
					if ($this->IsStateSupported($strEntity,"delete"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateDeleteSubmit(\"$strEntity\")");
						return $this->StateDeleteSubmit($strEntity);
					}				
					else
					{
						return $this->OnRenderModule($strEntity."_view"); 
					}
				}
				elseif ($strState == $strEntity."_export")
				{
					if ($this->IsStateSupported($strEntity,"export"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateExport(\"$strEntity\")");
						return $this->StateExport($strEntity);
					}
					else
					{
						return $this->OnRenderModule($strEntity."_list");
					}
				}
				elseif ($strState == $strEntity."_exportsubmit")
				{
					if ($this->IsStateSupported($strEntity,"export"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateExportSubmit(\"$strEntity\")");
						return $this->StateExportSubmit($strEntity);
					}
					else
					{
						return $this->OnRenderModule($strEntity."_list");
					}
				}				
				elseif ($strState == $strEntity."_import")
				{
					if ($this->IsStateSupported($strEntity,"import"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateImport(\"$strEntity\")");
						return $this->StateImport($strEntity);
					}
					else
					{
						return $this->OnRenderModule($strEntity."_list");
					}
				}
				elseif ($strState == $strEntity."_importsubmit")
				{
					if ($this->IsStateSupported($strEntity,"import"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateImportSubmit(\"$strEntity\")");
						return $this->StateImportSubmit($strEntity);
					}
					else
					{
						return $this->OnRenderModule($strEntity."_list");
					}
				}				
				
			}
			
			foreach ($arrayEntityDefinitions as $strEntity => $arrayEntityDefinition)
			{
				$bDefault = GetBoolValue(ArrayGetValue($arrayEntityDefinition,"default"));
				if ($bDefault == true)
				{
					if ($this->CheckCondition($strEntity,"list"))
					{
						$this->Trace("CEntityManagerModule::OnRenderModule(\"$strState\") -> calling StateList(\"$strEntity\")");
						return $this->StateList($strEntity);
					}
				}
			}
			$this->SetErrorMessageCode("UNSUPPORTEDSTATE");
			$this->SetState("");
			return "error";
		}

		function OnPrepareItemDataList($strEntity,&$arrayItems,$strSortOption,$strSortOrder)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$strIndexAttribute = ArrayGetValue($arrayEntity,"indexattribute");
			$nBlockSize = ArrayGetValue($arrayEntity,"blocksize");
			
			$nMaxOffset = $this->GetModuleVar($strEntity."_maxoffset");
			$nOffset = $this->GetModuleVar($strEntity."_offset");
			$nTotalEntries = intval($this->GetModuleVar($strEntity."_totalentries"));
			if ($nTotalEntries == 0)
			{
				$nTotalEntries = ArrayCount($arrayItems);
			}
			$nMoveMany = 5;
			
			foreach ($arrayItems as &$arrayItem)
			{
				$strIndex = ArrayGetValue($arrayItem,$strIndexAttribute);
				
				$this->OnPrepareItemDataView($strEntity,$strIndex,$arrayItem);
	
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					$strType = ArrayGetValue($arrayAttribute,"type");
					$strValue = ArrayGetValue($arrayItem,$strAttribute);
					$bZeroUnset = GetBoolValue(ArrayGetValue($arrayAttribute,"zerounset"));
					switch ($strType)
					{
					case "sortorder":
						
						if ($this->IsSortOrderTaskVisible($strEntity,$strAttribute) == true)
						{
							if ($strSortOption == $strAttribute)
							{
								$strTasks = "";
								//$strTasks = '<div style="white-space:nowrap;">';
								$nSortOrder = intval($strValue);
								
								$arrayParams = array();
								$arrayParams[$this->GetItemIndexPropertyModuleVarName($strEntity)] = $strIndex;
								$arrayParams[$this->GetModuleVarName("sortorder_attribute")] = $strAttribute;
		
								if ($nSortOrder > 0)
								{
									$arrayParams[$this->GetModuleVarName("sortorder_value")] = 0;
	
									$strTasks .= $this->RenderTaskIcon(
										$strEntity,
										"sortorderfirst",
										"chevron-up",
										"modifysortorder",
										$arrayParams);
								}

								if ($nSortOrder > 1)
								{
									$arrayParams[$this->GetModuleVarName("sortorder_value")] = $nSortOrder - 1;
									$strTasks .= $this->RenderTaskIcon(
										$strEntity,
										"sortorderup",
										"angle-up",
										"modifysortorder",
										$arrayParams);
								}

								if ($nSortOrder > $nMoveMany)
								{
									$arrayParams[$this->GetModuleVarName("sortorder_value")] = $nSortOrder - 5;
									$strTasks .= $this->RenderTaskIcon(
										$strEntity,
										"sortorderupmany",
										"angle-double-up",
										"modifysortorder",
										$arrayParams);
								}

		
								$strTasks .= "<br/>";

								if ($nSortOrder < $nTotalEntries-1)
								{
									$arrayParams[$this->GetModuleVarName("sortorder_value")] = INTEGER_MAX;
									$strTasks .= $this->RenderTaskIcon(
										$strEntity,
										"sortorderlast",
										"chevron-down",
										"modifysortorder",
										$arrayParams);
								}	
			
								if ($nSortOrder < $nTotalEntries-2)  
								{
									$arrayParams[$this->GetModuleVarName("sortorder_value")] = $nSortOrder + 1;
									$strTasks .= $this->RenderTaskIcon(
										$strEntity,
										"sortorderdown",
										"angle-down",
										"modifysortorder",
										$arrayParams);
								}
		
								if ($nSortOrder < $nTotalEntries-1-$nMoveMany)
								{
									$arrayParams[$this->GetModuleVarName("sortorder_value")] = $nSortOrder + 5;
									$strTasks .= $this->RenderTaskIcon(
										$strEntity,
										"sortorderdownmany",
										"angle-double-down",
										"modifysortorder",
										$arrayParams);
								}	


		
								//$strTasks .= '</div>';
											
								//$arrayItem[$strAttribute] = $nSortOrder."&nbsp;&nbsp;".$strTasks;
								$arrayItem[$strAttribute] = $strTasks;

							}
						}
						break;
						
					default:
						break;
					}
				}
			}
		}



		function OnPrepareItemDataView($strEntity,$strIndex,&$arrayItem)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$strType = ArrayGetValue($arrayAttribute,"type");
				$strControl = ArrayGetValue($arrayAttribute,"control");
				$strValue = ArrayGetValue($arrayItem,$strAttribute);
				$bZeroUnset = GetBoolValue(ArrayGetValue($arrayAttribute,"zerounset"));
				switch ($strType)
				{
				case "string":
					if ($strControl == "textarea")
					{
						$arrayItem[$strAttribute] = nl2br($strValue);
					}
					else
					{
						$strValueTID = $this->GetValueTID($strEntity,$strAttribute,$strValue,false);
						if ($strValueTID != "")
						{
							$arrayItem[$strAttribute] = $strValueTID;
						}
	
					}
					break;
					
				case "email":
					if ($strValue != "")
					{
						$link = new CLinkElement($this->GetWebsite(),"default");
						$arrayItem[$strAttribute] = $link->RenderLink("mailto:".$strValue,$strValue);
					}
					break;
					
				case "datetime":
					if (IsValidDateTime($strValue))
					{
						$arrayItem[$strAttribute] = $this->GetWebsite()->RenderDateTime($strValue);
					}
					else if ($strValue == "0000-00-00 00:00:00")
					{
						$arrayItem[$strAttribute] = "";
					}
					break;
					
				case "date":
					if (IsValidDate($strValue))
					{
						$arrayItem[$strAttribute] = $this->GetWebsite()->RenderDate($strValue);
					}
					else if ($strValue == "0000-00-00")
					{
						$arrayItem[$strAttribute] = "";
					}
					break;
					
				case "bool":
					if (IsBoolString($strValue))
					{
						$bValue = GetBoolValue($strValue);
						$arrayItem[$strAttribute] = ($bValue)?("?TID_ITEM_YES?"):("?TID_ITEM_NO?");
					}
					break;
					
				case "float":
					$fValue = floatval($strValue);
					if ($bZeroUnset == true && $fValue == 0)
					{
						$arrayItem[$strAttribute] = "";
					}
					else
					{
						$nPrecision = intval(ArrayGetValue($arrayAttribute,"floatprecision"));
						if ($nPrecision == 0)
						{
							$nPrecision = ENTITY_FLOATPRECISION_DEFAULT;
						}
						$arrayItem[$strAttribute] = $this->GetWebsite()->RenderNumber($fValue,$nPrecision);
					}
					break;

				case "integer":
					$nValue = intval($strValue);
					if ($bZeroUnset == true && $nValue == 0)
					{
						$arrayItem[$strAttribute] = "";
					}
					break;
					
				case "numericid":
					$nValue = intval($strValue);
					if ($bZeroUnset == true && $nValue == 0)
					{
						$arrayItem[$strAttribute] = "";
					}
					break;
					
				case "enum":
					$strValueTID = $this->GetValueTID($strEntity,$strAttribute,$strValue,true);
					if ($strValueTID != "")
					{
						$arrayItem[$strAttribute] = "$strValueTID";
					}
					break;

				case "sortorder":
					$nValue = intval($strValue);
					if ($bZeroUnset == true && $nValue == 0)
					{
						$arrayItem[$strAttribute] = "";
					}
					else if ($nValue == INTEGER_MAX)
					{
						$arrayItem[$strAttribute] = "";
					}
					break;
				
				case "language":
					if ($strValue != "")
					{
						$arrayItem[$strAttribute] = "?TID_LANGUAGE_".StringUpperCase($strValue)."?";
					}
					break;
					
				default:
					break;
				}
			}
		}

		function OnPrepareItemDataDefaultValues($strEntity,&$arrayItem)
		{
		}

		function OnPrepareItemDataDuplicate($strEntity,&$arrayItem)
		{
		}


		function OnPrepareItemDataModify($strEntity,$strIndex,&$arrayItem)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$strType = ArrayGetValue($arrayAttribute,"type");
				$strControl = ArrayGetValue($arrayAttribute,"control");
				$strValue = ArrayGetValue($arrayItem,$strAttribute);
				switch ($strType)
				{
				case "datetime":
					if (!IsValidDateTime($strValue))
					{
						$arrayItem[$strAttribute] = "";
					}
					else
					{
						$strTemp = $this->GetWebsite()->RenderDateTime($strValue);
						if (IsValidDateTime(ConvertToDateTime($strTemp)))
						{
							$arrayItem[$strAttribute] = $strTemp;
						}
					}
					break;
					
				case "date":
					if (!IsValidDate($strValue))
					{
						$arrayItem[$strAttribute] = "";
					}
					else
					{
						$strTemp = $this->GetWebsite()->RenderDate($strValue);
						if (IsValidDate(ConvertToDate($strTemp)))
						{
							$arrayItem[$strAttribute] = $strTemp;
						}
					}
					break;
				
				case "numericid":
					if (StringSection($strValue,0,1) == "_")
					{
						$strValue = StringSection($strValue,1);	
					}
					if (intval($strValue) == 0)
					{
						$arrayItem[$strAttribute] = "";
					}
					else
					{
						if ($strControl == "select")
						{
							$arrayItem[$strAttribute] = "_".$strValue;
						}
					}
					break;
				
				case "float":
					$nPrecision = intval(ArrayGetValue($arrayAttribute,"floatprecision"));
					if ($nPrecision == 0)
					{
						$nPrecision = ENTITY_FLOATPRECISION_DEFAULT;
					}
					$arrayItem[$strAttribute] = $this->GetWebsite()->RenderNumber(floatval($strValue),$nPrecision);
					break;
					
				default:
					break;
				}
			}	
		}


		
		function OnPrepareItemDataExport($strEntity,$strIndex,$arrayItem)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$strType = ArrayGetValue($arrayAttribute,"type");
				$strValue = ArrayGetValue($arrayItem,$strAttribute);
				switch ($strType)
				{
				case "string":
					// This is to prevent Excel from interpreting single-line strings starting with a dash as broken calculations
					if (FindString($strValue,"\n") == -1 && StringSection($strValue,0,1) == "-")
					{ 
						$arrayItem[$strAttribute] = " ".$strValue;
					}
					break;
				case "datetime":
					if (!IsValidDateTime($strValue))
					{
						$arrayItem[$strAttribute] = "";
					}
					break;
					
				case "date":
					if (!IsValidDate($strValue))
					{
						$arrayItem[$strAttribute] = "";
					}
					break;
					
				case "float":
					$nPrecision = intval(ArrayGetValue($arrayAttribute,"floatprecision"));
					if ($nPrecision == 0)
					{
						$nPrecision = 2;	
					}
					/*$arrayItem[$strAttribute] = $this->GetWebsite()->RenderNumber(
						floatval($strValue),
						$nPrecision,
						"",
						false);
						*/
					$arraySettings = array(
						"DECIMAL_DELIMITER" => $this->m_strExportDecimalDelimiter);
					$arrayItem[$strAttribute] = RenderNumber(
						floatval($strValue),
						$nPrecision,
						"",
						$arraySettings);
					break;
					
				default:
					break;
				}
			}
			return $arrayItem;
		}


		function OnPrepareItemDataImport($strEntity,$bAdd,$arrayItem)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bReadOnly = $this->IsAttributeReadOnly($strEntity,$strAttribute);
				if (!$bReadOnly)
				{
					$strType = ArrayGetValue($arrayAttribute,"type");
					if ($bAdd == true || ArrayKeyExists($arrayItem,$strAttribute))
					{
						$strValue = ArrayGetValue($arrayItem,$strAttribute);
						switch ($strType)
						{
						case "string":
							if ($strValue == "#NAME?")
							{
								$arrayItem = ArrayRemoveKey($arrayItem,$strAttribute);
							}
							else
							{
								$arrayItem[$strAttribute] = StringTrim($strValue);
							}
							break;

						case "email":
							break; 
						
						case "datetime":
							break;
							
						case "date":
							break;
							
						case "float":
							if ((FindString($strValue,",") != -1) && (FindString($strValue,".") == -1))
							{
								$strValue = ReplaceString($strValue,",",".");	
							}
							$arrayItem[$strAttribute] = floatval($strValue);
							break;
	
						case "integer":
							break;
							
						case "enum":
							break;
	
						default:
							break;
						}
					}
				}
			}		
			return $arrayItem;			
		}


		function OnCheckInputValues($strEntity,$bAdd,$arrayItem)
		{
			$arrayErrors = array();
			
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bReadOnly = $this->IsAttributeReadOnly($strEntity,$strAttribute);
				if (!$bReadOnly)
				{
					$strType = ArrayGetValue($arrayAttribute,"type");
					$bInputRequired = GetBoolValue(ArrayGetValue($arrayAttribute,"inputrequired"));
					if ($bAdd == true || ArrayKeyExists($arrayItem,$strAttribute))
					{
						$strValue = ArrayGetValue($arrayItem,$strAttribute);
						switch ($strType)
						{
						case "string":
							$nMinLength = intval(ArrayGetValue($arrayAttribute,"minlen"));
							$nMaxLength = intval(ArrayGetValue($arrayAttribute,"maxlen"));
							$nLength = StringLength($strValue);
							if ($nLength < $nMinLength)
							{
								$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_VALUE_TOO_SHORT?";
							}
							else if ($nLength > $nMaxLength)
							{
								$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_VALUE_TOO_LONG?";
							}
							else
							{
								$strCharSet = ArrayGetValue($arrayAttribute,"charset");						
								if ($strCharSet != "")
								{
									if (!StringCheckCharSet($strValue,$strCharSet))
									{
										$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_INVALID_CHARS?";
									}
								}
							}
							break;

						case "email":
							$nLength = StringLength($strValue);
							if ($nLength > EMAIL_MAXLEN)
							{
								$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_VALUE_TOO_LONG?";
							}
							else
							{
								if ($strValue != "")
								{
									if (!IsValidEmail($strValue))
									{
										$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_INVALID_EMAIL?";
									}
								}
								else
								{
									if ($bInputRequired == true)
									{
										$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_VALUE_TOO_SHORT?";
									}
								}
							}
							break; 
						
						case "datetime":
							if	($strValue != "")
							{
								if (!IsValidDateTime($strValue))
								{
									$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_INVALID_DATETIME?";
								}
							}
							else
							{
								if ($bInputRequired == true)	
								{
									$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_MISSING_DATETIME?";
								}
								
							}
							break;
							
						case "date":
							if	($strValue != "")
							{
								if (!IsValidDate($strValue))
								{
									$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_INVALID_DATE?";
								}
							}
							else
							{
								if ($bInputRequired == true)	
								{
									$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_MISSING_DATE?";
								}
							}
							break;
							
						case "float":
							if (!StringCheckCharSet($strValue,CHARSET_FLOAT_INTERNATIONAL))
							{
								$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_INVALID_FLOAT?";
							}
							else
							{
								$nPrecision = intval(ArrayGetValue($arrayAttribute,"floatprecision"));
								if ($nPrecision == 0)
								{
									$nPrecision = ENTITY_FLOATPRECISION_DEFAULT;
								}
								if (is_float($strValue))
								{
									$fValue = $strValue;
								}
								else
								{
									$fValue = GetLocalFloatValue(
										$strValue,
										$this->GetWebsite()->GetCountry(),
										$this->GetWebsite()->GetLanguage());
								}
								
								$fMin = floatval(ArrayGetValue($arrayAttribute,"min"));
								$fMax = floatval(ArrayGetValue($arrayAttribute,"max"));
								if ($fValue < $fMin)
								{
									$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_VALUE_TOO_LOW?";
								}
								else if ($fValue > $fMax)
								{
									$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_VALUE_TOO_HIGH?";
								}
								
							}
							break;
	
						case "integer":
							if (!StringCheckCharSet($strValue,CHARSET_INTEGER))
							{
								$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_INVALID_INTEGER?";
							}
							else
							{
								$nValue = intval($strValue);
								
								$nMin = intval(ArrayGetValue($arrayAttribute,"min"));
								$nMax = intval(ArrayGetValue($arrayAttribute,"max"));
								if ($nValue < $nMin)
								{
									$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_VALUE_TOO_LOW?";
								}
								else if ($nValue > $nMax)
								{
									$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_VALUE_TOO_HIGH?";
								}
								
							}
							break;
							
						case "enum":
							$arrayEnum = ArrayGetValue($arrayAttribute,"enum");
							if (ArrayValueExists($arrayEnum,$strValue) == false)
							{
								$arrayErrors[$strAttribute] = "?TID_ERROR_FORM_INVALID_ENUM?";
							}
							break;
	
						default:
						/*
							if (StringSection($strValue,0,1) == "_")
							{
								$arrayItem[$strAttribute] = StringSection($strValue,1);
							}
							*/
							break;
						}
					}
				}
			}
			return $arrayErrors;
		}


		function OnPrepareInputValues($strEntity,$bAdd,$arrayItem)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bReadOnly = $this->IsAttributeReadOnly($strEntity,$strAttribute);
				if (!$bReadOnly)
				{
					$strType = ArrayGetValue($arrayAttribute,"type");
					$bInputRequired = GetBoolValue(ArrayGetValue($arrayAttribute,"inputrequired"));
					if ($bAdd == true || ArrayKeyExists($arrayItem,$strAttribute))
					{
						$strValue = ArrayGetValue($arrayItem,$strAttribute);
						switch ($strType)
						{
						case "string":
							break;

						case "email":
							break; 
						
						case "datetime":
							if	($strValue != "")
							{
								if (!IsValidDateTime($strValue))
								{
									$strValue = ConvertToDateTime($strValue);
									if (IsValidDateTime($strValue))
									{
										$arrayItem[$strAttribute] = RenderDateTime(
											$strValue,						
											$this->GetWebsite()->GetTimeZone());	
									}
								}
							}
							break;
							
						case "date":
							if	($strValue != "")
							{
								if (!IsValidDate($strValue))
								{
									$strValue = ConvertToDateTime($strValue);
									if (IsValidDateTime($strValue))
									{
										$arrayItem[$strAttribute] = RenderDate(
											$strValue,						
											$this->GetWebsite()->GetTimeZone());
									}
								}
							}
							break;
							
						case "float":
							if (StringCheckCharSet($strValue,CHARSET_FLOAT_INTERNATIONAL))
							{
								$fValue = GetLocalFloatValue(
									$strValue,
									$this->GetWebsite()->GetCountry(),
									$this->GetWebsite()->GetLanguage());
	
								$nPrecision = intval(ArrayGetValue($arrayAttribute,"floatprecision"));
								if ($nPrecision == 0)
								{
									$nPrecision = ENTITY_FLOATPRECISION_DEFAULT;
								}
								$fValue = round($fValue,$nPrecision);
								$arrayItem[$strAttribute] = $fValue;
							}
							
							break;
	
						case "integer":
							break;
							
						case "enum":
							break;
	
						default:
							if (StringSection($strValue,0,1) == "_")
							{
								$arrayItem[$strAttribute] = StringSection($strValue,1);
							}
							break;
						}
					}
				}
			}
			return $arrayItem;		
		}


		
		function OnPrepareWebserviceParams($strEntity,&$arrayParams)
		{
		}
		
		function OnPrepareSelectOptions($strEntity,$strAttributeID,$strIndex)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttribute = ArrayGetValue($arrayEntity,"attributes",$strAttributeID);
			if (!is_array($arrayAttribute))
			{
				return false;	
			}
		
			$strType = ArrayGetValue($arrayAttribute,"type");
			
			$arrayOptions = array();
			
			if ($strType == "language")
			{
				$arrayOptions = array();
				$arrayLanguages = $this->GetSupportedLanguages();
				foreach ($arrayLanguages as $strLanguage)
				{
					$arrayOptions[$strLanguage] = $this->GetWebsite()->LoadString("TID_LANGUAGE_".StringUpperCase($strLanguage));
					if ($arrayOptions[$strLanguage] == "")
					{
						$arrayOptions[$strLanguage] = $strLanguage;
					}
				}
				$arrayOptions = ArraySort($arrayOptions,true);
				return $arrayOptions;
			}

			if ($strType == "currency")
			{
				$arrayOptions = array();
				$arrayCurrencies = $this->GetWebsite()->GetSupportedCurrencies();
				foreach ($arrayCurrencies as $strCurrency)
				{
					$arrayOptions[$strCurrency] = $this->GetWebsite()->LoadString("TID_CURRENCY_NAME_".StringUpperCase($strCurrency));
					if ($arrayOptions[$strCurrency] == "")
					{
						$arrayOptions[$strCurrency] = $strCurrency;
					}
				}
				$arrayOptions = ArraySort($arrayOptions,true);
				return $arrayOptions;
			}

			
			if ($strType == "enum")
			{
				$arrayEnum = ArrayGetValue($arrayAttribute,"enum");
				if (!is_array($arrayEnum))
				{
					return false;
				}
				foreach ($arrayEnum as $strEnum)
				{
					$strValueTID = $this->GetValueTID($strEntity,$strAttributeID,$strEnum);
					if ($strValueTID != "")
					{
						$arrayOptions[$strEnum] = $strValueTID;
					}
					else
					{
						$arrayOptions[$strEnum] = $strEnum;
					}
				}
				return $arrayOptions;
			}

			$strControl = ArrayGetValue($arrayAttribute,"control");
			if ($strControl != "select")
			{
				return false;	
			}

			$bInputRequired = GetBoolValue(ArrayGetValue($arrayAttribute,"inputrequired"));
			if ($bInputRequired == false)
			{
				$arrayOptions[""] = "?TID_OPTION_NONE?";
			}
			
			$arraySFW = ArrayGetValue($arrayAttribute,"selectfromwebservice");
			if (is_array($arraySFW) == true)
			{
				$strWebservice = ArrayGetValue($arraySFW,"webservice");
				$arrayParams = ArrayGetValue($arraySFW,"params");
				$strOptionField = ArrayGetValue($arraySFW,"optionfield");
				$strUserFriendlyField = ArrayGetValue($arraySFW,"userfriendlyfield");
				
				$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
				$webservice->ConsumeWebService($strWebservice,$arrayParams);
				$strError = $webservice->GetError();
				if ($strError != "")
				{
					$this->SetErrorMessageCodeEntity($strEntity,$strError);
					return $arrayOptions;
				}

				//$nMaxOffset = intval($webservice->GetResultValue("MAXOFFSET"));
				//if ()
				//$this->SetModuleVar($strEntity."_maxoffset",$nMaxOffset);
				//$nTotalEntries = intval($webservice->GetResultValue("TOTALENTRIES"));
				//$this->SetModuleVar($strEntity."_totalentries",$nMaxOffset);

				$arrayResultList = $webservice->GetResultList();
				foreach ($arrayResultList as $arrayResultItem)
				{
					$strOptionValue = ArrayGetValue($arrayResultItem,$strOptionField);
					if ($strOptionValue != "")
					{
						$strUserFriendlyValue = $strOptionValue;
						if ($strType == "numericid")
						{
							$strOptionValue = "_".$strOptionValue;
						}
						
						if ($strUserFriendlyField != "")
						{
							$strUserFriendlyValue = ArrayGetValue($arrayResultItem,$strUserFriendlyField);
						}
						$arrayOptions[$strOptionValue] = $strUserFriendlyValue;
					}
				}
				return $arrayOptions;
			}
			return false;
		}
		
		function OnPrepareFilterSelectOptions($strEntity,$strFilterID)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayFilter = ArrayGetValue($arrayEntity,"filteroptions","filters",$strFilterID);
			if (!is_array($arrayFilter))
			{
				return false;
			}
			$strWebserviceFilterOption = ArrayGetValue($arrayFilter,"webservicefilteroption");
			if ($strWebserviceFilterOption == "")
			{
				return false;
			}
			$arrayAttribute = ArrayGetValue($arrayEntity,"attributes",$strWebserviceFilterOption);
			if (!is_array($arrayAttribute))
			{
				return false;	
			}
			$strType = ArrayGetValue($arrayAttribute,"type");
			switch ($strType)
			{
			case "enum":
				$arrayOptions = array();
				$arrayOptions[""] = "?TID_OPTION_ALL?";
				$arrayOptions = ArrayMerge(
					$this->OnPrepareSelectOptions(
						$strEntity,
						$strWebserviceFilterOption,
						""),
					$arrayOptions);
				return $arrayOptions;
				
			case "bool":
				$arrayOptions[""] = "?TID_OPTION_ALL?";
				$arrayOptions["1"] = "?TID_ITEM_YES?";
				$arrayOptions["0"] = "?TID_ITEM_NO?";
				return $arrayOptions;	

			case "language":
				$arrayOptions = array();
				$arrayLanguages = $this->GetSupportedLanguages();
				foreach ($arrayLanguages as $strLanguage)
				{
					$arrayOptions[$strLanguage] = $this->GetWebsite()->LoadString("TID_LANGUAGE_".StringUpperCase($strLanguage));
				}
				$arrayOptions = ArraySort($arrayOptions,true);
				
				$arrayOptions = ArrayAppend(
					array("" => "?TID_OPTION_ALL?"),
					$arrayOptions);
				
				return $arrayOptions;
				
				

			default:
				$arraySFW = ArrayGetValue($arrayAttribute,"selectfromwebservice");
				if (ArrayCount($arraySFW))
				{
					$arrayOptions = array();
					// This is to make sure the all option is on top.
					// OnPrepareSelectOptions may add a "None" option.
					$arrayOptions[""] = "";
					$arrayOptions = ArrayAppend(
						$arrayOptions,
						$this->OnPrepareSelectOptions(
							$strEntity,
							$strWebserviceFilterOption,
							""));
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					return $arrayOptions;
				}			
			
				return false;
			}
			return false;
		}		
	
		function OnPrepareToolbar()
		{

			$strState = $this->GetState();
			$arrayEntitys = $this->GetEntityDefinitions();
			foreach ($arrayEntitys as $strEntity => $arrayEntity)
			{
				$strEntityIndex = $this->GetItemIndexPropertyModuleVar($strEntity);
				
				if ($strState != $strEntity."_list")
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_list";
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					$this->AddTool($strLink, "list", "?".$this->MakeEntityTID($strEntity,"TOOL","LIST")."?");
				}

				if ($this->IsStateSupported($strEntity,"add"))
				{
					if ($strState != $strEntity."_add")
					{
						$arrayParams = array();
						$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_add";
						$strLink = $this->GetWebsite()->MakeURL($arrayParams);
						$this->AddTool($strLink, "add", "?".$this->MakeEntityTID($strEntity,"TOOL","ADD")."?");
					}
				}

				if ($strState == $strEntity."_list")
				{
					if (ArrayGetValue($arrayEntity,"list","refresh") == true)
					{
						$arrayParams = array();
						$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_list";
						$strLink = $this->GetWebsite()->MakeURL($arrayParams);
						$this->AddTool($strLink, "refresh", "?".$this->MakeEntityTID($strEntity,"TOOL","REFRESH")."?");
					}
				}

				if ($strEntityIndex != "")
				{
					if (($strState != $strEntity."_list") && ($strState != $strEntity."_add"))
					{
						if ($this->IsStateSupported($strEntity,"view"))
						{
							if (($strState != $strEntity."_view") /*&& ($strState != $strEntity."_duplicate")*/)
							{
								$arrayParams = array();
								$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_view";
								$arrayParams[$this->GetItemIndexPropertyModuleVarName($strEntity)] = $strEntityIndex;
								$strLink = $this->GetWebsite()->MakeURL($arrayParams);
								$this->AddTool($strLink, "view", "?".$this->MakeEntityTID($strEntity,"TOOL","VIEW")."?");
							}						
						}
						if ($this->IsStateSupported($strEntity,"modify"))
						{
							if (($strState != $strEntity."_modify") && ($strState != $strEntity."_duplicate"))
							{
								$arrayParams = array();
								$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_modify";
								$arrayParams[$this->GetItemIndexPropertyModuleVarName($strEntity)] = $strEntityIndex;
								$strLink = $this->GetWebsite()->MakeURL($arrayParams);
								$this->AddTool($strLink, "modify", "?".$this->MakeEntityTID($strEntity,"TOOL","MODIFY")."?");
							}	
						}
						if ($this->IsStateSupported($strEntity,"duplicate"))
						{
							if ($strState != $strEntity."_duplicate")
							{
								$arrayParams = array();
								$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_duplicate";
								$arrayParams[$this->GetItemIndexPropertyModuleVarName($strEntity)] = $strEntityIndex;
								$strLink = $this->GetWebsite()->MakeURL($arrayParams);
								$this->AddTool($strLink, "duplicate", "?".$this->MakeEntityTID($strEntity,"TOOL","DUPLICATE")."?");
							}	
						}
						if ($this->IsStateSupported($strEntity,"delete"))
						{	
							if (($strState != $strEntity."_delete") && ($strState != $strEntity."_duplicate"))
							{
								$arrayParams = array();
								$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_delete";
								$arrayParams[$this->GetItemIndexPropertyModuleVarName($strEntity)] = $strEntityIndex;
								$strLink = $this->GetWebsite()->MakeURL($arrayParams);
								$this->AddTool($strLink, "delete", "?".$this->MakeEntityTID($strEntity,"TOOL","DELETE")."?");
							}
						}
					}
				}
				if ($this->IsStateSupported($strEntity,"export"))
				{
					//if ($strState == $strEntity."_list")
					//{
						$arrayParams = array();
						$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_export";
						$strLink = $this->GetWebsite()->MakeURL($arrayParams);
						$this->AddTool($strLink, "export", "?".$this->MakeEntityTID($strEntity,"TOOL","EXPORT")."?");
					//}
				}
				if ($this->IsStateSupported($strEntity,"import"))
				{
					//if ($strState == $strEntity."_list")
					//{
						$arrayParams = array();
						$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_import";
						$strLink = $this->GetWebsite()->MakeURL($arrayParams);
						$this->AddTool($strLink, "import", "?".$this->MakeEntityTID($strEntity,"TOOL","IMPORT")."?");
					//}
				}
				
				if ($strEntityIndex != "")
				{
					if ($this->IsStateSupported($strEntity,"view"))
					{
						if ($strState == $strEntity."_view")
						{
							if (ArrayGetValue($arrayEntity,"view","refresh") == true)
							{
								$arrayParams = array();
								$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_view";
								$arrayParams[$this->GetItemIndexPropertyModuleVarName($strEntity)] = $strEntityIndex;
								$strLink = $this->GetWebsite()->MakeURL($arrayParams);
								$this->AddTool($strLink, "refresh", "?".$this->MakeEntityTID($strEntity,"TOOL","REFRESH")."?");
							}
						}						
					}						
				}
			}
		}
		
		function OnItemAdded($strEntity,$strNewItemIndex,$arrayItem)
		{
			$arrayItem["INDEX"] = $strNewItemIndex;
			$this->GetWebsite()->SystemEvent("INFORMATIONAL",StringUpperCase($strEntity)."_ADD",$strNewItemIndex,$arrayItem);
		}
		
		function OnItemModified($strEntity,$strItemIndex,$arrayItem)
		{
			$arrayItem["INDEX"] = $strItemIndex;
			$this->GetWebsite()->SystemEvent("INFORMATIONAL",StringUpperCase($strEntity)."_MODIFY",$strItemIndex,$arrayItem);
		}

		function OnItemSortOrderModified($strEntity,$strItemIndex,$arrayItem)
		{
			$arrayItem["INDEX"] = $strItemIndex;
			$this->GetWebsite()->SystemEvent("INFORMATIONAL",StringUpperCase($strEntity)."_MODIFYSORTORDER",$strItemIndex,$arrayItem);
		}


		function OnItemDuplicated($strEntity,$strItemIndex,$arrayItem)
		{
			$arrayItem["INDEX"] = $strItemIndex;
			$this->GetWebsite()->SystemEvent("INFORMATIONAL",StringUpperCase($strEntity)."_DUPLICATE",$strItemIndex,$arrayItem);
		}
		
		function OnItemDeleted($strEntity,$strItemIndex,$arrayItem)
		{
			$arrayItem["INDEX"] = $strItemIndex;
			$this->GetWebsite()->SystemEvent("INFORMATIONAL",StringUpperCase($strEntity)."_DELETE",$strItemIndex,$arrayItem);
		}		
		
		function OnItemView($strEntity,$strItemIndex,$arrayItem)
		{
		}
		
		function OnGetExportFileTitle($strEntity)
		{
			//return "export";
			return $strEntity."_".StringTranslateCharacters(
				RenderDateTimeFormat(
					GetTimeNow(),
					FORMAT_DATETIME." %Z",
					$this->GetWebsite()->GetTimeZone()),
					": ","-_"); 
		}


		function RenderTaskIcon($strEntity,$strTaskID,$strIconID,$strState,$arrayParams)
		{
			if ($this->m_linkicon == null)
			{
				$this->m_linkicon = new CLinkIconElement($this->GetWebsite(),"default");
			}
			$strState = $strEntity . "_".$strState;
			$strTooltip = "?".$this->MakeEntityTID($strEntity,"TOOLTIP",$strTaskID)."?";

			if (!is_array($arrayParams))
			{
				$arrayParams = array();	
			}
			$arrayParams[$this->GetModuleVarNameState()] = $strState;
			$strTaskLink = $this->GetWebsite()->MakeURL($arrayParams);
			return $this->m_linkicon->RenderLinkIcon(
				$strTaskLink,
				$strIconID,
				$strTooltip);			
		}
		
		final
		function AddTool($strLink,$strIconID,$strText,$bExternal = false)
		{
			$this->m_toolbarDef->AddTool($strLink, $strIconID, $strText,$bExternal);
		}

		final
		function RenderToolbar()
		{
			$this->OnPrepareToolbar();
			if ($this->m_toolbarDef->GetToolCount() == 0)
			{
				$this->SetOutputItem("TOOLBAR",""); 
			}
			else
			{
				$toolbar = new CToolbarElement($this->GetWebsite(),"default");
				$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($this->m_toolbarDef)); 
			}
		}

		final
		function GetFormErrors()
		{
			if (!is_array($this->m_arrayErrors))
			{
				$this->m_arrayErrors = array();	
			}
			return ArrayKeysUpperCase($this->m_arrayErrors);
		}
		
		final
		function SetFormErrors($arrayErrors)
		{
			if (is_array($arrayErrors) == false)
			{
				$arrayErrors = array();	
			}
			$this->m_arrayErrors = $arrayErrors;
		}

		function StateList($strEntity)
		{
			$this->Trace("StateList(\"$strEntity\")");
			$this->SetState($strEntity."_list");
			$this->AssertValidEntity($strEntity);
			
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$bDefault = GetBoolValue(ArrayGetValue($arrayEntity,"default"));
			$strWebserviceParamPrefix = ($bDefault)?(""):($strEntity."_");
			
			// ITEMMANAGER
			$itemmanagerdata = new CItemManagerData();

			// GET OFFSET	
			$nOffset = intval($this->GetModuleVar($strEntity."_offset"));
			
			// GET SORT OPTIONS
			$strSortDefaultAttribute = ArrayGetValue($arrayEntity,"sorting","defaultattribute");
			$strSortOption = "";
			if ($strSortDefaultAttribute != "")
			{
				$strSortOption = $this->GetModuleVar($strEntity."_sort");
				if ($strSortOption == "")
				{
					 $strSortOption = $strSortDefaultAttribute;
					 //$this->Trace($strSortOption);
				}
				$strSortOrder = $this->GetModuleVar($strEntity."_sort_order");
				if ($strSortOrder == "")
				{
					$strSortOrder = ArrayGetValue($arrayEntity,"sorting","defaultorder");
				}
			}
			
			// GET BLOCKSIZE
			$nBlockSize = ArrayGetValue($arrayEntity,"blocksize");

			// INVOKE WEBSERVICE PART 1
			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = array();
			if ($nBlockSize != 0) 
			{
				$arrayParams["blocksize"] = $nBlockSize;
				$arrayParams["offset"] = $nOffset;
			}
			if ($strSortOption != "")
			{
				$arrayParams[$strWebserviceParamPrefix."sort1"] = $strSortOption;
				$arrayParams[$strWebserviceParamPrefix."sort1_order"] = $strSortOrder;
			}
			
			// FILTER OPTIONS
			
			$arrayFilterOptions = ArrayGetValue($arrayEntity,"filteroptions");
			if (!is_array($arrayFilterOptions))
			{
				$arrayFilterOptions = array();
			}
			$arrayFilters = ArrayGetValue($arrayFilterOptions,"filters");
			if (!is_array($arrayFilters))
			{
				$arrayFilters = array();
			}
			
			
			$strWebserviceFilterParam = $strWebserviceParamPrefix."filter";
			$nFilterIndex = 1;
			
			// Context filters
			$arrayContextFilters = $this->GetContextFilters($strEntity);
			foreach ($arrayContextFilters as $strContextFilterAttribute => $strContextFilterValue)
			{
				$arrayParams[$strWebserviceFilterParam.$nFilterIndex] = $strContextFilterAttribute;
				$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_operator"] = "=";
				$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_value"] = $strContextFilterValue;
				$nFilterIndex++;		
			}
			
			if (ArrayCount($arrayFilters) > 0)
			{
				$formDef = new CFormDefinition();
				$formDef->SetURL(
					$this->GetWebsite()->GetRootURL(),
					$this->GetWebsite()->GetSessionAwareArrayVars());
					
				$bFilterReset = GetBoolValue(ArrayGetValue($arrayFilterOptions,"reset"));
			
				if ($bFilterReset == true)
				{
					$arrayFilterResetParams = array();
					foreach ($arrayFilters as $strFilter => $arrayFilter)
					{
						$arrayFilterResetParams[$this->GetModuleVarName($strEntity."_filter_".$strFilter)] = "";
					}
					$strLinkReset = $this->GetWebsite()->MakeURL($arrayFilterResetParams);
					
					$strResetToolTip = ArrayGetValue($arrayFilterOptions,"resettooltip");
					$strResetIcon = ArrayGetValue($arrayFilterOptions,"reseticon");
					
					if ($this->m_linkiconButton == null)
					{
						$this->m_linkiconButton = new CLinkIconElement($this->GetWebsite(),"button");	
					}
					
					$strLinkIconHtml = $this->m_linkiconButton->RenderLinkIcon(
						$strLinkReset,
						$strResetIcon,
						$strResetToolTip);
					
					$formDef->AddControlStatic(
						"FILTERRESET",
						$strLinkIconHtml);
				}
				
				$nFilterOptions = 0;
				foreach ($arrayFilters as $strFilter => $arrayFilter)
				{
					$strType = ArrayGetValue($arrayFilter,"type");
					$strLabel = ArrayGetValue($arrayFilter,"label");
					$strWebserviceFilterOption = ArrayGetValue($arrayFilter,"webservicefilteroption");
					
					// If the filter is configured as a context filter, we ignore it!
					if ($this->IsAttributeInContextFilters($strEntity,$strWebserviceFilterOption) == true)
					{
						continue;
					}
					
					$strModuleVarName = $strEntity."_filter_".$strFilter;
					switch ($strType)
					{
						case "search":
						{
							$strFilterModuleVar = $this->GetModuleVarInsecure($strModuleVarName);
							if ($strFilterModuleVar != "")
							{
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex] = $strWebserviceFilterOption;
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_operator"] = "contains";
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_value"] = $strFilterModuleVar;
							}
	
							$formDef->AddControlText(
								StringUpperCase($strFilter),
								$this->GetModuleVarName($strModuleVarName),
								$strLabel,
								$this->GetModuleVar($strModuleVarName),
								20,
								20);

							break;
						}
						case "select":
						{ 
							$strFilterModuleVar = $this->GetModuleVarInsecure($strModuleVarName);
							if ($strFilterModuleVar != "")
							{
								if ( (StringSection($strFilterModuleVar,0,1)=="_") && (is_numeric(StringSection($strFilterModuleVar,1))) )
								{
									$strFilterModuleVar = StringSection($strFilterModuleVar,1);
								}

								$arrayParams[$strWebserviceFilterParam.$nFilterIndex] = $strWebserviceFilterOption;
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_operator"] = "=";
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_value"] = $strFilterModuleVar;
							}
							
							$arrayOptions = $this->OnPrepareFilterSelectOptions(
								$strEntity,
								$strFilter);
								
							if (!is_array($arrayOptions))
							{
								$this->Error("Implementation error in OnPrepareFilterSelectOptions(\"$strEntity\",\"$strFilter\"). The function must return an array!");	
							}
							if (ArrayCount($arrayOptions) == 0)
							{
								$this->Error("Implementation error in OnPrepareFilterSelectOptions(\"$strEntity\",\"$strFilter\"). The function must return a non-empty array!");	
							}
							$formDef->AddControlSelect(
								StringUpperCase($strFilter),
								$this->GetModuleVarName($strModuleVarName),
								$strLabel,
								$arrayOptions,
								$this->GetModuleVar($strModuleVarName));
							break;
						}
						default:
						{
							$this->Error("Filter type \"$strType\" is not supported!");
							break;
						}
					}
					$bSeparator = GetBoolValue(ArrayGetValue($arrayFilter,"separator"));
					if ($bSeparator == true)
					{
						$formDef->AddControlSeparator();
					}
					$nFilterIndex++;	
					$nFilterOptions++;
				}

				if ($nFilterOptions > 0)
				{				
					$strSubmitButtonText = ArrayGetValue($arrayFilterOptions,"submitbuttontext");
					$strFormStyle = ArrayGetValue($arrayFilterOptions,"formstyle");
					$strFormID = ArrayGetValue($arrayFilterOptions,"formid");
					$formDef->AddControlButtonSubmit($strSubmitButtonText);
					$itemmanagerdata->SetFilterOptionsForm(
						$formDef,
						$strFormStyle,
						$strFormID);
				}
			}
			

			// INVOKE WEBSERVICE PART 2
			
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"list");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);
				return "error";
			}


			// MAXOFFSET AND TOTALENTRIES
			$nMaxOffset = intval($webservice->GetResultValue("MAXOFFSET"));
			$this->SetModuleVar($strEntity."_maxoffset",$nMaxOffset);
			$nTotalEntries = intval($webservice->GetResultValue("TOTALENTRIES"));
			$this->SetModuleVar($strEntity."_totalentries",$nTotalEntries);


			// OFFSET TOO HIGH?
			if ($nOffset > $nMaxOffset)
			{
				$this->SetModuleVar($strEntity."_offset",$nMaxOffset);
				return $this->StateList($strEntity);
			}			
			



 			// ADD COLUMNS
 			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bHideInList = $this->IsAttributeHiddenInList($strEntity,$strAttribute);
				if ($bHideInList == false)
				{
					$strSortOptionParam = "";
					$bSortOption = GetBoolValue(ArrayGetValue($arrayAttribute,"sortoption"));
					if ($bSortOption == true)
					{ 
						$strSortOptionParam = $strAttribute;
					}
					$bShareColumn = GetBoolValue(ArrayGetValue($arrayAttribute,"sharecolumn"));
					$strTableHeader = ArrayGetValue($arrayAttribute,"tableheader");
					if ($strTableHeader == "")
					{
						$strTableHeader = $this->LoadString($this->MakeEntityTID($strEntity,"TABLEHEADER",$strAttribute));
					}
					if ($strTableHeader == "")
					{
						$strTableHeader = $this->LoadString($this->MakeEntityTID($strEntity,"LABEL",$strAttribute));
					}
					$strTab = ArrayGetValue($arrayAttribute,"tab");;
					$itemmanagerdata->AddColumn(
						$strAttribute,
						$strTableHeader,
						$strSortOptionParam,
						$bShareColumn,
						$strTab);
				}
			}
			
			// TABS
			$arrayTabs = ArrayGetValue($arrayEntity,"tabs");
			$strCurrentTab = $this->GetModuleVar($strEntity."_tab");
			$itemmanagerdata->SetColumnsTabNavigation(
				$strCurrentTab,
				$this->GetModuleVarName($strEntity."_tab"),
				$arrayTabs);			
			
			
			// ADD SORT OPTIONS
			if ($strSortDefaultAttribute != "")
			{
				$itemmanagerdata->SetSortOptions(
					$this->GetModuleVarName($strEntity."_sort"),
					$strSortOption,
					$this->GetModuleVarName($strEntity."_sort_order"),
					$strSortOrder
					);
			}
		
			// ADD ITEM DATA
			$arrayItems = $this->GetItemListFromWebservice($strEntity,$webservice);
			
			// INDEX ATTRIBUTE
			$strIndexProperty = ArrayGetValue($arrayEntity,"indexattribute");
			$arrayIndexProperty = ArrayGetValue($arrayEntity,"attributes",$strIndexProperty);
			$strIndexPropertyWebserviceresult = StringUpperCase($strIndexProperty);
						
			// PREPARE TASKS
			$arrayTasks = ArrayGetValue($arrayEntity,"tasks");
			if (!is_array($arrayTasks))
			{
				$arrayTasks = array();	
			}
			$bRenderTasks = GetBoolValue(ArrayCount($arrayTasks) > 0);
			if ($bRenderTasks == true)
			{ 
				$itemmanagerdata->AddColumn("TASKS",$this->LoadStringEntity($strEntity,"TABLEHEADER","TASKS"));
			}
			
			$strIndexState = ArrayGetValue($arrayEntity,"indexstate");
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");

			foreach ($arrayItems as &$arrayItem)
			{
				$strIndex = $arrayItem[$strIndexProperty];

				if ($bRenderTasks == true)	
				{
					$strTasks = "";
					foreach ($arrayTasks as $strTaskID => $arrayTask)
					{
						if ($this->CheckCondition($strEntity,$strTaskID) == true)
						{
							$strState = ArrayGetValue($arrayTask,"state");
							if ($this->IsStateSupported($strEntity,$strState))
							{
								$strState = $strEntity . "_" . $strState;
								$strIconID = ArrayGetValue($arrayTask,"icon");
								$strTooltip = ArrayGetValue($arrayTask,"tooltip");
								if ($strTooltip == "")
								{
									$strTooltip = "?".$this->MakeEntityTID($strEntity,"TOOLTIP",$strTaskID)."?";
								}
							
								$arrayParams = array();
								$arrayParams[$this->GetModuleVarNameState()] = $strState;
								$arrayParams[$this->GetModuleVarName($strIndexProperty)] = $strIndex;
								
								$strTaskLink = $this->GetWebsite()->MakeURL($arrayParams);
								if ($this->m_linkicon == null)
								{
									$this->m_linkicon = new CLinkIconElement($this->GetWebsite(),"default");	
								}
								$strTasks .= $this->m_linkicon->RenderLinkIcon(
									$strTaskLink,
									$strIconID,
									$strTooltip);
							}
						}
					}
					$arrayItem["TASKS"] = $strTasks;
				}
			}
			
			$this->OnPrepareItemDataList($strEntity,$arrayItems,$strSortOption,$strSortOrder);

			foreach ($arrayItems as &$arrayItem)
			{
				//$strIndex = $arrayItem[$strIndexProperty];
				$strIndex = $arrayItem["INDEX"];
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					$bIndexLink = GetBoolValue(ArrayGetValue($arrayAttribute,"indexlink"));
					if ($bIndexLink == true)
					{
						if ($strIndexState != "")
						{
							if ($this->m_link == null)
							{
								$this->m_link = new CLinkElement($this->GetWebsite(),"default");
							}
							$strLinkText = $arrayItem[$strAttribute];
							if ($strLinkText != "")
							{
								$arrayParams = array();
								$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_".$strIndexState;
								$arrayParams[$this->GetModuleVarName($strIndexProperty)] = $strIndex;
								$strIndexLink = $this->GetWebsite()->MakeURL($arrayParams);
								$arrayItem[$strAttribute] = $this->m_link->RenderLink($strIndexLink,$strLinkText);
							}
						}
					}
				}
			}
			
			// SET ITEM DATA
			$itemmanagerdata->SetItemList($arrayItems);


			// SET PAGE NAVIGATION
			$nPageCount = intval($webservice->GetResultValue("MAXOFFSET")) + 1;
			$itemmanagerdata->SetPageNavigation(
				$nOffset,
				$nPageCount,
				$this->GetModuleVarName($strEntity."_offset"),
				20);

			// RENDER ITEM MANAGER
			$itemmanager = new CItemManagerElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"ITEMMANAGER",
				$itemmanager->RenderItemManager(
					$itemmanagerdata));
		
			// NO ITEMS FOUND MESSAGE
			if (ArrayCount($arrayItems) == 0)
			{
				$strMessage = ArrayGetValue($arrayEntity,"noitemsfoundmessage");				
				$message = new CMessageElement($this->GetWebsite(),"info");
				$this->SetOutputItem(
					"NOITEMSFOUNDMESSAGE",
					$message->RenderMessage($strMessage)
					);
			}
			else
			{
				$this->SetOutputItem("NOITEMSFOUNDMESSAGE","");
			}
		
			// RENDER TOOLBAR
			$this->RenderToolbar();
			
			return "list";
		}
		
		private
		function GetExportColumnHeaderFromAttribute($strAttributeID,$arrayAttribute,$bUserFriendlyColumnHeaders)
		{
			if (!is_array($arrayAttribute))
			{
				return "";	
			}
			$strColumnHeader = ArrayGetValue($arrayAttribute,"exportcolumnheader");
			if ($bUserFriendlyColumnHeaders == true)
			{
				if ($strColumnHeader == "")
				{
					$strColumnHeader = ArrayGetValue($arrayAttribute,"tableheader");
				}
				if ($strColumnHeader == "")
				{
					$strColumnHeader = $this->MakeEntityTID($strEntity,"TABLEHEADER",$strAttributeID);
				}
				$strColumnHeader = $this->GetWebsite()->ReplaceText($strColumnHeader);
			}
			else
			{
				if ($strColumnHeader == "")
				{
					$strColumnHeader = $strAttributeID;
				}
				$strColumnHeader = StringUpperCase($strColumnHeader);
			}			
			return $strColumnHeader;
		}
		
		
		function StateExport($strEntity)
		{
			$this->Trace("StateImport(\"$strEntity\")");
			$this->SetState($strEntity."_export");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);			


			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_exportsubmit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);


			$bHasFloat = false;
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			foreach ($arrayAttributes as $arrayAttribute)
			{
				if (ArrayGetValue($arrayAttribute,"type") == "float")
				{
					$bHasFloat = true;
					break;	
				}
			}


			$strDecimalDelimiter = $this->GetVar("export_decimal_delimiter");
			if ($strDecimalDelimiter == "")
			{
				$strDecimalDelimiter = $this->GetConfig("exportdecimaldelimiter");
			}
			if ($bHasFloat == true)
			{
				$arrayOptionsDecimalDelimiter = array(
					"." => "?TID_OPTION_EXPORT_DECIMAL_DELIMITER_DOT?",
					"," => "?TID_OPTION_EXPORT_DECIMAL_DELIMITER_COMMA?");
	
				$formDef->AddControlRadio(
					"DECIMALDELIMITER",
					"export_decimal_delimiter",
					"?TID_LABEL_EXPORT_DECIMAL_DELIMITER?",
					$arrayOptionsDecimalDelimiter,
					$strDecimalDelimiter);
			}
			else
			{
				$formDef->SetHiddenValue("export_decimal_delimiter",$strDecimalDelimiter);
			}

			$formDef->AddControlButtonSubmitDownload($this->LoadStringEntity($strEntity,"BUTTON","EXPORT"));

			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
					$formDef,
					array(),
					$this->GetFormErrors()));
			
			$this->RenderToolbar();
			
			return "export";
		}
		
		

		function StateExportSubmit($strEntity)
		{
			$this->Trace("StateExport(\"$strEntity\")");
			$this->SetState($strEntity."_export");
			$this->AssertValidEntity($strEntity);
			
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$strIndexAttribute = ArrayGetValue($arrayEntity,"indexattribute");	
			$bDefault = GetBoolValue(ArrayGetValue($arrayEntity,"default"));
			$nBlockSize = ArrayGetValue($arrayEntity,"blocksize");
			$strWebserviceParamPrefix = ($bDefault)?(""):($strEntity."_");

			// GET DECIMAL DELIMITER
			$this->m_strExportDecimalDelimiter = $this->GetVar("export_decimal_delimiter");
			
			// GET SORT OPTIONS
			$strSortDefaultAttribute = ArrayGetValue($arrayEntity,"sorting","defaultattribute");
			$strSortOption = "";
			if ($strSortDefaultAttribute != "")
			{
				$strSortOption = $this->GetModuleVar($strEntity."_sort");
				if ($strSortOption == "")
				{
					 $strSortOption = $strSortDefaultAttribute;
				}
				$strSortOrder = $this->GetModuleVar($strEntity."_sort_order");
				if ($strSortOrder == "")
				{
					$strSortOrder = "asc";
				}
			}
			
			// PREPARE WEB SERVICE
			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());


			$arrayParams = array();
			
			// BLOCK SIZE
			if ($nBlockSize != 0)
			{
				$arrayParams["blocksize"] = $nBlockSize;	
			}
			
			// SORT OPTIONS
			if ($strSortOption != "")
			{
				$arrayParams[$strWebserviceParamPrefix."sort1"] = $strSortOption;
				$arrayParams[$strWebserviceParamPrefix."sort1_order"] = $strSortOrder;
			}

			// FILTER OPTIONS
			$arrayFilterOptions = ArrayGetValue($arrayEntity,"filteroptions");
			if (!is_array($arrayFilterOptions))
			{
				$arrayFilterOptions = array();
			}
			$arrayFilters = ArrayGetValue($arrayFilterOptions,"filters");
			if (!is_array($arrayFilters))
			{
				$arrayFilters = array();
			}
			if (ArrayCount($arrayFilters) > 0)
			{
				$strWebserviceFilterParam = $strWebserviceParamPrefix."filter";
				$nFilterIndex = 1;
				foreach ($arrayFilters as $strFilter => $arrayFilter)
				{
					$strType = ArrayGetValue($arrayFilter,"type");
					$strWebserviceFilterOption = ArrayGetValue($arrayFilter,"webservicefilteroption");
					$strModuleVarName = $strEntity."_filter_".$strFilter;
					switch ($strType)
					{
						case "search":
						{
							$strFilterModuleVar = $this->GetModuleVarInsecure($strModuleVarName);
							if ($strFilterModuleVar != "")
							{
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex] = $strWebserviceFilterOption;
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_operator"] = "contains";
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_value"] = $strFilterModuleVar;
							}
						break;
						}
						case "select":
						{
							$strFilterModuleVar = $this->GetModuleVarInsecure($strModuleVarName);
							if ($strFilterModuleVar != "")
							{
								if ( (StringSection($strFilterModuleVar,0,1)=="_") && (is_numeric(StringSection($strFilterModuleVar,1))) )
								{
									$strFilterModuleVar = StringSection($strFilterModuleVar,1);
								}
								
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex] = $strWebserviceFilterOption;
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_operator"] = "=";
								$arrayParams[$strWebserviceFilterParam.$nFilterIndex."_value"] = $strFilterModuleVar;
							}
							
							break;
						}
						default:
						{
							$this->Error("Filter type \"$strType\" is not supported!");
							break;
						}
					}
					$nFilterIndex++;	
				}
			}
			
			$nOffset = 0;
			$arrayListRaw = array();
			do
			{
				$arrayParams["offset"] = $nOffset;
		
				// INVOKE WEB SERVICE
				$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"list");
				$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
				$webservice->ConsumeWebService($strWebservice,$arrayParams);
				$strError = $webservice->GetError();
				if ($strError != "")
				{
					$this->SetErrorMessageCodeEntity($strEntity,$strError);
					return "error";
				}

				// GET MAX OFFSET
				$nMaxOffset = intval($webservice->GetResultValue("MAXOFFSET"));
						
				// ADD ITEM DATA
				$arrayListRaw = ArrayAppend($arrayListRaw,$webservice->GetResultListInsecure());
				
				$nOffset++;
				
			} while ($nOffset-1 <= $nMaxOffset);
			
			$arrayItemList = array();
			
			
			// Prepare Column Headers
			$arrayColumnHeaders = array();
			$bUserFriendlyColumnHeaders = GetBoolValue(ArrayGetValue($arrayEntity,"userfriendlyexportcolumnheaders"));
			foreach ($arrayAttributes as $strAttributeID => $arrayAttribute)
			{
				$bHideInExport	= GetBoolValue(ArrayGetValue($arrayAttribute,"hideinexport"));
				if (!$bHideInExport)
				{
					$strColumnHeader = $this->GetExportColumnHeaderFromAttribute(
						$strAttributeID,
						$arrayAttribute,
						$bUserFriendlyColumnHeaders);
					$arrayColumnHeaders[$strAttributeID] = $strColumnHeader;
				}
			}
			
			
			foreach ($arrayListRaw as $arrayItemRaw)
			{
				$arrayItem = array();
				$strIndex = "";
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					$bHideInExport	= GetBoolValue(ArrayGetValue($arrayAttribute,"hideinexport"));
					if (!$bHideInExport)
					{
						$strTag = StringUpperCase($strAttribute);
						$arrayItem[$strAttribute] = ArrayGetValue($arrayItemRaw,$strTag);
					}
					if ($strAttribute == $strIndexAttribute)
					{
						$strIndex = $arrayItemRaw[$strTag];
					}
				}
				$arrayItem = $this->OnPrepareItemDataExport($strEntity,$strIndex,$arrayItem);
				$arrayItem2 = array();
				foreach ($arrayAttributes as $strAttributeID => $arrayAttribute)
				{
					$bHideInExport	= GetBoolValue(ArrayGetValue($arrayAttribute,"hideinexport"));
					if (!$bHideInExport)
					{
						$strColumnHeader = ArrayGetValue($arrayColumnHeaders,$strAttributeID);
						$arrayItem2[$strColumnHeader] = ArrayGetValue($arrayItem,$strAttributeID);
					}
				}
				array_push($arrayItemList,$arrayItem2);
			}
			
			
		
		/*
			// NO ITEMS FOUND MESSAGE
			if (ArrayCount($arrayResultItemList) == 0)
			{
				$strMessage = ArrayGetValue($arrayEntity,"noitemsfoundmessage");				
				$message = new CMessageElement($this->GetWebsite(),"info");
				$this->SetOutputItem(
					"NOITEMSFOUNDMESSAGE",
					$message->RenderMessage($strMessage)
					);
			}
			*/
		
		
			$strCSV = $this->GetWebsite()->MakeCSV($arrayItemList);
			
			$this->Trace($strCSV);
		
			$this->GetWebsite()->DownloadFile(
				"text/csv",
				$this->OnGetExportFileTitle($strEntity).".csv",
				$strCSV);
			
				
			return $this->StateList($strEntity);
				
		}


		function StateImport($strEntity)
		{
			$this->Trace("StateImport(\"$strEntity\")");
			$this->SetState($strEntity."_import");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);			


			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_importsubmit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$formDef->AddControlFile(
				"IMPORTFILE",
				$this->GetModuleVarName($strEntity."_importfile"),
				"?".$this->MakeEntityTID($strEntity,"LABEL","IMPORTFILE")."?");  
			
			$formDef->AddControlButtonSubmit($this->LoadStringEntity($strEntity,"BUTTON","IMPORT")); 

			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
					$formDef,
					array(),
					$this->GetFormErrors()));
			
			$this->RenderToolbar();
			
			return "import";
		}



		function StateImportSubmit($strEntity)
		{
			$this->Trace("StateImportSubmit(\"$strEntity\")");
			$this->SetState($strEntity."_importsubmit");
			$this->AssertValidEntity($strEntity);
			
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$strIndexAttribute = ArrayGetValue($arrayEntity,"indexattribute");	
			$strUserFriendlyAttribute = ArrayGetValue($arrayEntity,"userfriendlyattribute");	
			$bDefault = GetBoolValue(ArrayGetValue($arrayEntity,"default"));
			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$strWebserviceParamPrefix = ($bDefault)?(""):($strEntity."_");

			$strFileID = $this->GetModuleVarName($strEntity."_importfile");
			if (GetUploadedFileName($strFileID) == "")
			{
				$this->m_arrayErrors["IMPORTFILE"] = "?TID_ERROR_".StringUpperCase($strEntity)."_NOIMPORTFILE?";
				return $this->StateImport($strEntity);	
			}
			
			$strFileContents = GetUploadedFileContents($strFileID);
			
			$arrayImport = $this->GetWebsite()->ParseCSV($strFileContents);
	
			if (!is_array($arrayImport))
			{
				$this->m_arrayErrors["IMPORTFILE"] = "?TID_ERROR_".StringUpperCase($strEntity)."_IMPORTFILEINVALID?";
			}
			
			$bUserFriendlyColumnHeaders = GetBoolValue(ArrayGetValue($arrayEntity,"userfriendlyexportcolumnheaders"));
			$arrayColumnHeaders = array();
			foreach ($arrayAttributes as $strAttributeID => $arrayAttribute)
			{
				$strExportColumnHeader = $this->GetExportColumnHeaderFromAttribute(
					$strAttributeID,
					$arrayAttribute,
					$bUserFriendlyColumnHeaders);
				$arrayColumnHeaders[$strExportColumnHeader] = $strAttributeID;
			}
			


		
			$arrayItems = array();
			foreach ($arrayImport as $arrayImportItem)
			{
				$arrayItem = array();
				foreach ($arrayImportItem as $strKey => $strValue)
				{
					$strAttributeID = ArrayGetValue($arrayColumnHeaders,$strKey);
					if ($strAttributeID != "")
					{
						$arrayItem[$strAttributeID] = $strValue;
					}
				}
				$arrayItems[] = $arrayItem;
			}
			
			$bError = false;
			foreach ($arrayItems as &$arrayItem)
			{
				$strIndexValue = ArrayGetValue($arrayItem,$strIndexAttribute);
				if ($strIndexValue == "")
				{
					$bAdd = true;
				}
				else
				{
					$bAdd = false;	
				}

				$arrayItem = $this->OnPrepareItemDataImport($strEntity,$bAdd,$arrayItem);  
				$arrayItem = $this->OnPrepareInputValues($strEntity,$bAdd,$arrayItem);
				$arrayErrors = $this->OnCheckInputValues($strEntity,$bAdd,$arrayItem);
				
				$strUserFriendlyValue = ArrayGetValue($arrayItem,$strUserFriendlyAttribute);
				if (ArrayCount($arrayErrors) > 0)
				{
					$bError = true;
					foreach ($arrayErrors as $strAttribute => $strError)
					{
						$this->GetWebsite()->AddErrorMessageText(
							$strUserFriendlyValue.": "
							."?".$this->MakeEntityTID($strEntity,"LABEL",$strAttribute)."?"
							.": ".$strError);	
					}
				}
			}
			if ($bError == true)
			{
				$this->AddErrorMessageCodeEntity($strEntity,"IMPORT");
				return $this->StateImport($strEntity);
			}

			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			
			foreach ($arrayItems as $arrayItem2)
			{
				$strIndexValue = ArrayGetValue($arrayItem2,$strIndexAttribute);
				if ($strIndexValue == "")
				{
					$bAdd = true;
				}
				else
				{
					$bAdd = false;	
				}
				
				$arrayParams = array();
				$arrayParams = $this->GetItemWebserviceParams($strEntity,$arrayItem2);
				if ($bAdd == true)
				{
					$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"add");
				}
				else
				{
					$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"set");					
					$arrayParams[$strIndexAttribute] = $strIndexValue;
				}
				
				
				$webservice->ConsumeWebService($strWebservice,$arrayParams);
				$strError = $webservice->GetError();
				if ($strError != "")
				{
					$this->AddErrorMessageCodeEntity($strEntity,"WRITE_FAILED");
					return $this->StateImport($strEntity);
				}
			}
			
			$this->AddInfoMessageCodeEntity($strEntity,"IMPORT_COMPLETE");
			$this->SetModuleVar($strEntity."_importfile","");
			return $this->StateList($strEntity);
		}



		
		function StateView($strEntity)
		{
			$this->Trace("StateView(\"$strEntity\")");
			$this->SetState($strEntity."_view");
			
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$strEntityIndex = $this->GetItemIndexPropertyModuleVar($strEntity);
			if ($strEntityIndex == "")
			{
				return $this->StateList($strEntity);
			}
			
			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			
			$arrayParams = array();
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"get");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);
				return $this->StateList($strEntity);
			}

			$arrayItem = $this->GetItemValuesFromWebservice($strEntity,$webservice);
			
			$strStateConditionFailed = $this->CheckConditionForItem($strEntity,$arrayItem,"view");
			if ($strStateConditionFailed != "")
			{
				return $strStateConditionFailed;
			}
			
			$arrayItemRaw = $arrayItem;
			$this->OnPrepareItemDataView($strEntity,$strEntityIndex,$arrayItem);
			
			$strViewStyle = ArrayGetValue($arrayEntity,"view","style");
			if ($strViewStyle == "")
			{
				$strViewStyle = "vertical";
			}
			$bHideEmpty = GetBoolValue(ArrayGetValue($arrayEntity,"view","hideempty"));
			$bHideHeader = GetBoolValue(ArrayGetValue($arrayEntity,"view","hideheader"));
			
			$arrayTableData = array();
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			
			
			switch ($strViewStyle)
			{
			case "custom":
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					$bHideInView = $this->IsAttributeHiddenInView($strEntity,$strAttribute);
					if ($bHideInView == false)
					{
						$strLabel = ArrayGetValue($arrayAttribute,"formlabel");
						if ($strLabel == "")
						{
							$strLabel = "?".$this->MakeEntityTID($strEntity,"LABEL",$strAttribute)."?";  
						}
						$strValue = $arrayItem[$strAttribute];
						
						$this->SetOutputItem(
							StringUpperCase($strAttribute)."_LABEL",
							$strLabel);

						$this->SetOutputItem(
							StringUpperCase($strAttribute),
							$strValue);
					}
				}
				break;

			case "text":
				$arrayCollection = $this->PrepareTextView($strEntity,$arrayItem,1,$bHideEmpty);
				$collection = new CCollectionElement($this->GetWebsite(),"default");
				$strOutput = $collection->RenderCollection($arrayCollection,1);   
				$this->SetOutputItem(
					"ITEM",
					$strOutput);
				break;
			
			case "horizontal":
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					$bHideInView = $this->IsAttributeHiddenInView($strEntity,$strAttribute);
					if ($bHideInView == false)
					{
						$strLabel = ArrayGetValue($arrayAttribute,"formlabel");
						if ($strLabel == "")
						{
							$strLabel = "?".$this->MakeEntityTID($strEntity,"LABEL",$strAttribute)."?";  
						}
						$strValue = $arrayItem[$strAttribute];
						if ($strValue != "" || $bHideEmpty == false)
						{
							$arrayRow = array($strLabel,$strValue);
							array_push($arrayTableData,$arrayRow);
						}
					}
				}
				$arrayTableData = ArrayTableRotate($arrayTableData);				
				$table = new CTableElement($this->GetWebsite(),"default");
				$this->SetOutputItem(
					"ITEM",
					$table->RenderTable($arrayTableData,true,false));
				break;				
			
			case "vertical":
			default:
				$arrayCollection = $this->PrepareVerticalTableView($strEntity,$arrayItem,$bHideEmpty,$bHideHeader);
				$collection = new CCollectionElement($this->GetWebsite(),"default");
				$strOutput = $collection->RenderCollection($arrayCollection,1);
				$this->SetOutputItem(
					"ITEM",
					$strOutput);
				break;
			}
			
			$this->RenderToolbar();	
			
			$strState = $this->OnItemView($strEntity,$strEntityIndex,$arrayItemRaw);
			if ($strState != "")
			{
				return $strState;	
			}
			return "view";
		}
		
		
		function PrepareTextView($strEntity,$arrayItem,$nHeaderLevel=1,$bHideEmpty = false)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$strUserFriendlyAttribute = ArrayGetValue($arrayEntity,"userfriendlyattribute");
			
			$arrayCollection = array(
				array(
					"element" => "header",
					"headerlevel" => $nHeaderLevel,
					"header" => $arrayItem[$strUserFriendlyAttribute]
					)
				);
			
			$arrayTabs = ArrayAppend(array("" => ""),ArrayGetValue($arrayEntity,"tabs"));
			$arrayTableData = array();			
			foreach ($arrayTabs as $strCurrentTab => $strTabLabel)
			{
				$bTabHeaderAdded = false;
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					$bHideInView = $this->IsAttributeHiddenInView($strEntity,$strAttribute);
					if ($bHideInView == false && $strAttribute != $strUserFriendlyAttribute)
					{
						$strTab = ArrayGetValue($arrayAttribute,"tab");
						if ($strTab == $strCurrentTab)
						{
							$strValue = $arrayItem[$strAttribute];
							if ($strValue != "" || $bHideEmpty == false)
							{
								if ($bTabHeaderAdded == false && $strCurrentTab != "")
								{
									$arrayCollection[] = array(
										"element" => "header",
										"headerlevel" => $nHeaderLevel+1,
										"header" => $strTabLabel);
									$bTabHeaderAdded = true;
								}
								$strLabel = "?".$this->MakeEntityTID($strEntity,"LABEL",$strAttribute)."?";  
								$strControl = ArrayGetValue($arrayAttribute,"control");
								if ($strControl == "textarea")
								{
									if (ArrayCount($arrayTableData) > 0)
									{
										$arrayCollection[] = array(
											"element" => "table",
											"headerfirstcol" => true,
											"data" => $arrayTableData);
										$arrayTableData = array();
									}
									
									/*
									$arrayCollection[] = array(
										"element" => "header",
										"headerlevel" => $nHeaderLevel+2,
										"header" => $strLabel);
									$arrayCollection[] = array(
										"element" => "paragraph",
										"paragraph" => $strValue);
									*/
									$arrayCollection[] = array(
										"element" => "text",
										"headline" => $strLabel,    
										"text" => $strValue);
								}
								else
								{
									$arrayTableData[] = array(
										$strLabel,
										$strValue
										);
								}
							}
						}
					}
				}
				if (ArrayCount($arrayTableData) > 0)
				{
					$arrayCollection[] = array(
						"element" => "table",
						"headerfirstcol" => true,
						"data" => $arrayTableData);
				}
				$arrayTableData = array();
			}
			return $arrayCollection;	
		}

		function PrepareVerticalTableView($strEntity,$arrayItem,$bHideEmpty = false,$bHideHeader = false)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$strUserFriendlyAttribute = ArrayGetValue($arrayEntity,"userfriendlyattribute");

			if ($bHideHeader == false)
			{
				$arrayCollection = array(
					array(
						"element" => "header",
						"headerlevel" => "1",
						"header" => $arrayItem[$strUserFriendlyAttribute]
						)
					);
			}

			$arrayTableData = array();
			$arrayTableHeaders = array();
			
			$arrayTabs = ArrayAppend(
				array("" => ""),
				ArrayGetValue($arrayEntity,"tabs"));

			foreach ($arrayTabs as $strCurrentTab => $strTabLabel)
			{
				$bTabHeaderAdded = false;
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					$strTab = ArrayGetValue($arrayAttribute,"tab");
					if ($strTab == $strCurrentTab)
					{
						$bHideInView = $this->IsAttributeHiddenInView($strEntity,$strAttribute);
						if ($bHideInView == false)
						{
							if ($strAttribute != $strUserFriendlyAttribute || $bHideHeader == true)
							{
								$strValue = $arrayItem[$strAttribute];
								if ($strValue != "" || $bHideEmpty == false)
								{
									if ($bTabHeaderAdded == false && $strTab != "")
									{
										array_push($arrayTableData,
											array(
												"",  
												$strTabLabel
												));
										array_push($arrayTableHeaders,array(true,true));
										$bTabHeaderAdded = true;
									}
									
									$strLabel = ArrayGetValue($arrayAttribute,"formlabel");
									if ($strLabel == "")
									{
										$strLabel = "?".$this->MakeEntityTID($strEntity,"LABEL",$strAttribute)."?";  
									}
									array_push($arrayTableData,array($strLabel,$strValue));
									array_push($arrayTableHeaders,array(true,false));
								}
							}
						}
					}
				}
			}
			$arrayCollection[] = array(
				"element" => "table",
				"headers" => $arrayTableHeaders,
				"data" => $arrayTableData);

			return $arrayCollection;	
		}
		

		function StateAdd($strEntity)
		{
			$this->Trace("StateAdd(\"$strEntity\")");
			$this->SetState($strEntity."_add");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$formDef = new CFormDefinition();
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_addsubmit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			//$arrayItem = $this->GetItemDataFromModuleVars($strEntity);
			$arrayItem = $this->GetItemDataFromModuleVarsInsecure($strEntity);
			$arrayItem = $this->OnPrepareInputValues($strEntity,true,$arrayItem);
			
			$arrayItemDefaults = $this->GetItemDataDefaultValues($strEntity);
			$arrayItem = ArrayAppend($arrayItemDefaults,$arrayItem);
			
			// Override configured context filters
			$arrayContextFilters = $this->GetContextFilters($strEntity);
			$arrayItem = ArrayAppend($arrayItem,$arrayContextFilters);
			
			$arrayErrors = $this->AddItemControls(
				$strEntity,
				$formDef,
				$arrayItem,
				"");
			if (ArrayCount($arrayErrors) > 0)
			{
				foreach ($arrayErrors as $strAttribute => $strError)
				{
					$this->AddErrorMessageCodeEntity($strEntity,$strAttribute."_".$strError);
				}
				return $this->StateList($strEntity);
			}		
				
			$formDef->AddControlButtonSubmit("?".$this->MakeEntityTID($strEntity,"BUTTON","ADD")."?");

			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
					$formDef,
					array(),
					$this->GetFormErrors()));

			$this->ResetItemModuleVars($strEntity);
			$this->RenderToolbar();
			
			return "add";
		}
		
		function StateAddSubmit($strEntity)
		{
			$this->Trace("StateAddSubmit(\"$strEntity\")");
			$this->SetState($strEntity."_addsubmit");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$arrayItem = $this->GetItemDataFromModuleVarsInsecure($strEntity);
			$arrayItem = $this->OnPrepareInputValues($strEntity,true,$arrayItem);
			$this->m_arrayErrors = $this->OnCheckInputValues($strEntity,true,$arrayItem);
			if (ArrayCount($this->m_arrayErrors) > 0)
			{
				return $this->StateAdd($strEntity);
			}
	

			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");						
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = $this->GetItemWebserviceParams(
				$strEntity,
				$arrayItem);
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"add");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);					
				return $this->StateAdd($strEntity);
			}

			$strNewItemIndex = $webservice->GetResultValue(ArrayGetValue($arrayEntity,"webservice","newitemidtag"));
			$this->ResetItemModuleVars($strEntity);
			$this->OnItemAdded($strEntity,$strNewItemIndex,$arrayItem);

			// This is to ensure proper rendering of the user friendly attribute
			$this->OnPrepareItemDataView($strEntity,$strNewItemIndex,$arrayItem);
			$strUserFriendly = ArrayGetValue($arrayItem,ArrayGetValue($arrayEntity,"userfriendlyattribute"));
			$this->SetInfoMessageCodeEntity($strEntity,"ADD_SUCCESS");
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_USERFRIENDLY",$strUserFriendly);
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_INDEX",$strNewItemIndex);
			
			
			return $this->StateList($strEntity);
		}

		function StateModify($strEntity)
		{
			$this->Trace("StateModify(\"$strEntity\")");
			$this->SetState($strEntity."_modify");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$strEntityIndex = $this->GetItemIndexPropertyModuleVar($strEntity);
			if ($strEntityIndex == "")
			{
				return $this->StateList($strEntity);
			}
		
			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			
			$arrayParams = array();
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"get");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);
				return $this->StateList($strEntity);
			}

			$arrayItem = $this->GetItemValuesFromWebservice($strEntity,$webservice);
		
			$strStateConditionFailed = $this->CheckConditionForItem($strEntity,$arrayItem,"modify");
			if ($strStateConditionFailed != "")
			{
				return $strStateConditionFailed;
			}

		
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_modifysubmit";
			$arrayParams[$this->GetItemIndexPropertyModuleVarName($strEntity)] = $strEntityIndex;
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$arrayErrors = $this->AddItemControls(
				$strEntity,
				$formDef,
				$arrayItem,
				$strEntityIndex);
			if (ArrayCount($arrayErrors) > 0)
			{
				foreach ($arrayErrors as $strAttribute => $strError) 
				{
					$this->AddErrorMessageCodeEntity($strEntity,$strAttribute."_".$strError);
				}
				return $this->StateList($strEntity);
			}			
			
			$formDef->AddControlButtonSubmit("?".$this->MakeEntityTID($strEntity,"BUTTON","MODIFY")."?");
		
			$arrayItemDataVars = array();
			// This is to keep (possibly incorrect) values that have been entered in a previous iteration.
			if ((ArrayCount($this->GetFormErrors()) > 0) || ($this->GetWebsite()->IsError() == true))
			{
				$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
				$arrayItem = $this->GetItemDataFromModuleVarsInsecure($strEntity);
				// reconvert item ids back to module vars
				$arrayItemDataVars = array();
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					if (!$this->IsAttributeReadOnly($strEntity,$strAttribute))
					{
						$strModuleVarName = $this->GetModuleVarName($strAttribute);
						$arrayItemDataVars[$strModuleVarName] = ArrayGetValue($arrayItem,$strAttribute);
					}
				}
			}
		
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
					$formDef,
					$arrayItemDataVars,
					$this->GetFormErrors()));
			
			$this->ResetItemModuleVars($strEntity);
			
			$this->RenderToolbar();	
			
			return "modify";
		}

		function StateModifySubmit($strEntity) 
		{
			$this->Trace("StateModifySubmit(\"$strEntity\")");
			$this->SetState($strEntity."_modifysubmit");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$strEntityIndex = $this->GetItemIndexPropertyModuleVar($strEntity);
			if ($strEntityIndex == "")
			{
				return $this->StateList($strEntity);
			}
	

			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	


			// PREPARE MODIFIED VALUES			
			$arrayItem = $this->GetItemDataFromModuleVarsInsecure($strEntity);
			$arrayItem = $this->OnPrepareInputValues($strEntity,false,$arrayItem);
			$this->m_arrayErrors = $this->OnCheckInputValues($strEntity,false,$arrayItem);
			if (ArrayCount($this->m_arrayErrors) > 0)
			{
				return $this->StateModify($strEntity);
			}
			
			// INVOKE MODIFY COMMAND
			$arrayParams = $this->GetItemWebserviceParams(
				$strEntity,
				$arrayItem);
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"set");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);	
			}
			else
			{
				
				// GET USER FRIENDLY NAME	
				$arrayParams = array();
				$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
				$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"get");
				$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
				$webservice->ConsumeWebService($strWebservice,$arrayParams);
				$strError = $webservice->GetError();
				if ($strError != "")
				{
					$this->SetErrorMessageCodeEntity($strEntity,$strError);
					return $this->StateList($strEntity);
				}
				$arrayItem = $this->GetItemValuesFromWebservice($strEntity,$webservice);

				$this->OnItemModified($strEntity,$strEntityIndex,$arrayItem);

				// This is to ensure proper rendering of the user friendly attribute
				$this->OnPrepareItemDataView($strEntity,$strEntityIndex,$arrayItem);
				$strUserFriendly = ArrayGetValue($arrayItem,ArrayGetValue($arrayEntity,"userfriendlyattribute"));
				$this->SetInfoMessageCodeEntity($strEntity,"MODIFY_SUCCESS");
				$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_USERFRIENDLY",$strUserFriendly);
				$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_INDEX",$strEntityIndex);
				
				$this->ResetItemModuleVars($strEntity);
			}
						
			return $this->StateModify($strEntity);
		}



		function StateModifySortOrder($strEntity) 
		{
			$this->Trace("StateModifySortOrder(\"$strEntity\")");
			$this->SetState($strEntity."_modifysortorder");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$strEntityIndex = $this->GetItemIndexPropertyModuleVar($strEntity);
			if ($strEntityIndex == "")
			{
				return $this->StateList($strEntity);
			}
			if (!$this->IsModuleParam("sortorder_value"))
			{
				return $this->StateList($strEntity);
			}
			$nSortOrder = intval($this->GetModuleParam("sortorder_value"));
			$strSortOrderAttribute = $this->GetModuleParam("sortorder_attribute");
			if ($strSortOrderAttribute == "")
			{
				return $this->StateList($strEntity);
			}

			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	

			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"set");
			$arrayParams[$strSortOrderAttribute] = $nSortOrder;
			
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);	
			}
			else
			{
				// GET USER FRIENDLY NAME	
				$arrayParams = array();
				$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
				$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"get");
				$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
				$webservice->ConsumeWebService($strWebservice,$arrayParams);
				$strError = $webservice->GetError();
				if ($strError != "")
				{
					$this->SetErrorMessageCodeEntity($strEntity,$strError);
					return $this->StateList($strEntity);
				}
				$arrayItem = $this->GetItemValuesFromWebservice($strEntity,$webservice);
				$strUserFriendly = $arrayItem[ArrayGetValue($arrayEntity,"userfriendlyattribute")];
				
				$this->SetInfoMessageCodeEntity($strEntity,"MODIFYSORTORDER_SUCCESS");
				$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_USERFRIENDLY",$strUserFriendly);
				$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_INDEX",$strEntityIndex);
				$this->OnItemSortOrderModified($strEntity,$strEntityIndex,$arrayItem);
				
				$this->ResetItemModuleVars($strEntity);
			}
			
			// TODO: SORTORDER implementation
			// identify the right block to display 
						
			return $this->StateList($strEntity);
		}





		function StateDuplicate($strEntity)
		{
			$this->Trace("StateDuplicate(\"$strEntity\")");
			$this->SetState($strEntity."_duplicate");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$strEntityIndex = $this->GetItemIndexPropertyModuleVar($strEntity);
			if ($strEntityIndex == "")
			{
				return $this->StateList($strEntity);
			}

		
			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			
			$arrayParams = array();
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"get");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);
				return $this->StateList($strEntity);
			}

			$arrayItem = $this->GetItemValuesFromWebservice($strEntity,$webservice);
		
			$strStateConditionFailed = $this->CheckConditionForItem($strEntity,$arrayItem,"duplicate");
			if ($strStateConditionFailed != "")
			{
				return $strStateConditionFailed;
			}
			
			
			$this->OnPrepareItemDataDuplicate($strEntity,$arrayItem);

		
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_duplicatesubmit";
			$arrayParams[$this->GetItemIndexPropertyModuleVarName($strEntity)] = $strEntityIndex;
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$this->AddItemControls(
				$strEntity,
				$formDef,
				$arrayItem,
				"");
			
			$formDef->AddControlButtonSubmit("?".$this->MakeEntityTID($strEntity,"BUTTON","DUPLICATE")."?");
		
			$arrayItemDataVars = array();
			// This is to keep (possibly incorrect) values that have been entered in a previous iteration.
			if ((ArrayCount($this->GetFormErrors()) > 0) || ($this->GetWebsite()->IsError() == true))
			{
				$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
				$arrayItem = $this->GetItemDataFromModuleVarsInsecure($strEntity);
				// reconvert item ids back to module vars
				$arrayItemDataVars = array();
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					if (!$this->IsAttributeReadOnly($strEntity,$strAttribute))
					{
						$strModuleVarName = $this->GetModuleVarName($strAttribute);
						$arrayItemDataVars[$strModuleVarName] = ArrayGetValue($arrayItem,$strAttribute);
					}
				}
			}
		
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
					$formDef,
					$arrayItemDataVars,
					$this->GetFormErrors()));
			
			$this->ResetItemModuleVars($strEntity);
			
			$this->RenderToolbar();	
			
			return "duplicate";
		}

		function StateDuplicateSubmit($strEntity)
		{
			$this->Trace("StateAddSubmit(\"$strEntity\")");
			$this->SetState($strEntity."_addsubmit");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$arrayItem = $this->GetItemDataFromModuleVarsInsecure($strEntity);
			$arrayItem = $this->OnPrepareInputValues($strEntity,true,$arrayItem);
			$this->m_arrayErrors = $this->OnCheckInputValues($strEntity,true,$arrayItem);
			if (ArrayCount($this->m_arrayErrors) > 0)
			{
				return $this->StateDuplicate($strEntity);
			}
	

			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");						
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = $this->GetItemWebserviceParams(
				$strEntity,
				$arrayItem);
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"add");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);					
				return $this->StateDuplicate($strEntity);
			}

			$strNewItemIndex = $webservice->GetResultValue(ArrayGetValue($arrayEntity,"webservice","newitemidtag"));

			$this->ResetItemModuleVars($strEntity);

			$this->OnItemDuplicated($strEntity,$strNewItemIndex,$arrayItem);
		
			// This is to ensure proper rendering of the user friendly attribute
			$this->OnPrepareItemDataView($strEntity,$strNewItemIndex,$arrayItem);
			$strUserFriendly = ArrayGetValue($arrayItem,ArrayGetValue($arrayEntity,"userfriendlyattribute"));
			$this->SetInfoMessageCodeEntity($strEntity,"DUPLICATE_SUCCESS");
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_USERFRIENDLY",$strUserFriendly);
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_INDEX",$strNewItemIndex);

			
			return $this->StateList($strEntity);
		}

		function StateDelete($strEntity)
		{
			$this->Trace("StateDelete(\"$strEntity\")");
			$this->SetState($strEntity."_delete");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);			
			
			$strEntityIndex = $this->GetItemIndexPropertyModuleVar($strEntity);
			if ($strEntityIndex == "")
			{
				return $this->StateList($strEntity);	
			}

			$bDeleteWithoutConfirmation = GetBoolValue(ArrayGetValue($arrayEntity,"deletewithoutconfirmation"));
			if ($bDeleteWithoutConfirmation)
			{
				$this->SetModuleVar($strEntity."_deleteconfirm",true);
				return $this->StateDeleteSubmit($strEntity);	
			}



			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"get");
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);
				return $this->StateList($strEntity);
			}
			$arrayItem = $this->GetItemValuesFromWebservice($strEntity,$webservice);
			
			$strStateConditionFailed = $this->CheckConditionForItem($strEntity,$arrayItem,"delete");
			if ($strStateConditionFailed != "")
			{
				return $strStateConditionFailed;
			}
			
			
			// This is to ensure proper rendering of the user friendly attribute
			$this->OnPrepareItemDataView($strEntity,$strEntityIndex,$arrayItem);
			$strUserFriendly = ArrayGetValue($arrayItem,ArrayGetValue($arrayEntity,"userfriendlyattribute"));
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_USERFRIENDLY",$strUserFriendly);
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_INDEX",$strEntityIndex);

			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_deletesubmit";
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$formDef->AddControlCheckbox(
				"DELETECONFIRM",
				$this->GetModuleVarName($strEntity."_deleteconfirm"),
				"?".$this->MakeEntityTID($strEntity,"LABEL","DELETECONFIRM")."?",
				false);
			
						
			$formDef->AddControlButtonSubmit("?".$this->MakeEntityTID($strEntity,"BUTTON","DELETE")."?");

			$arrayFormErrors = array();
			if ($this->m_bErrorDeleteConfirm == true)
			{
				$arrayFormErrors["DELETECONFIRM"] = "?TID_".StringUpperCase($strEntity)."_ERROR_DELETE_CONFIRMATIONMISSING?";
			}

			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem("FORM",$form->RenderForm($formDef,array(),$arrayFormErrors));
			
			$this->RenderToolbar();
			
			return "delete";
		}



		function StateDeleteSubmit($strEntity)
		{
			$this->Trace("StateDeleteSubmit(\"$strEntity\")");
			$this->SetState($strEntity."_deletesubmit");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);			

			$strEntityIndex = $this->GetItemIndexPropertyModuleVar($strEntity);
			if ($strEntityIndex == "")
			{
				return $this->StateList($strEntity);	
			}
			
			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"get");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);
				return $this->StateList($strEntity);
			}
			$arrayItem = $this->GetItemValuesFromWebservice($strEntity,$webservice);

			$strStateConditionFailed = $this->CheckConditionForItem($strEntity,$arrayItem,"deletesubmit");
			if ($strStateConditionFailed != "")
			{
				return $strStateConditionFailed;
			}
			
			
			
			
			// This is to ensure proper rendering of the user friendly attribute
			$this->OnPrepareItemDataView($strEntity,$strEntityIndex,$arrayItem);
			$strUserFriendly = ArrayGetValue($arrayItem,ArrayGetValue($arrayEntity,"userfriendlyattribute"));
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_USERFRIENDLY",$strUserFriendly);
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_INDEX",$strEntityIndex);
		

			$bDeleteConfirm = $this->GetModuleVar($strEntity."_deleteconfirm");
			if ($bDeleteConfirm != true)
			{
				$this->m_bErrorDeleteConfirm = true;
				return $this->StateDelete($strEntity);
			}
						
			
			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"delete");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);	
			}
			else
			{
				$this->SetInfoMessageCodeEntity($strEntity,"DELETE_SUCCESS");
				$this->OnItemDeleted($strEntity,$strEntityIndex,$arrayItem);
			}
					
					
			return $this->StateList($strEntity);
		}
	
		
		final
		function GetItemDataFromModuleVars($strEntity, $bInsecure=false)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$arrayItem = array();
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bReadOnly = $this->IsAttributeReadOnly($strEntity,$strAttribute);
				if (!$bReadOnly)
				{
					if ($this->IsModuleVar($strAttribute) != false)
					{
						if ($bInsecure == true)
						{
							$arrayItem[$strAttribute] = $this->GetModuleVarInsecure($strAttribute);
						}
						else
						{
							$arrayItem[$strAttribute] = $this->GetModuleVar($strAttribute);
						}
					}
				}
			}
			return $arrayItem;
		}
		
		final
		function GetItemDataFromModuleVarsInsecure($strEntity)
		{		
			return $this->GetItemDataFromModuleVars($strEntity,true);
		}

		final
		function ResetItemModuleVars($strEntity)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bReadOnly = $this->IsAttributeReadOnly($strEntity,$strAttribute);
				if (!$bReadOnly)
				{				
					$this->SetModuleVar($strAttribute,"");
				}
			}
		}	
		
		final
		function GetItemWebserviceParams($strEntity,$arrayValues)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$arrayParams = array();
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bReadOnly = $this->IsAttributeReadOnly($strEntity,$strAttribute);
				if (!$bReadOnly)
				{
					if (ArrayKeyExists($arrayValues,$strAttribute))
					{
						$strParamName = $strAttribute;
						$arrayParams[$strParamName] = ArrayGetValue($arrayValues,$strAttribute);
					}
				}
			}
			return $arrayParams;
		}

		final
		function GetItemDataDefaultValues($strEntity)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$arrayItem = array();
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bReadOnly = $this->IsAttributeReadOnly($strEntity,$strAttribute);
				if ($bReadOnly == false)
				{
					$strDefault = $this->GetModuleConfig("defaultvalues",$strAttribute);
					if ($strDefault == "")
					{
						$strDefault = ArrayGetValue($arrayAttribute,"default");
					}
					if ($strDefault != "")
					{
						$arrayItem[$strAttribute] = $strDefault;
					}
				}
			}
			$this->OnPrepareItemDataDefaultValues($strEntity,$arrayItem);
			return $arrayItem;
		}


		
		final
		function AddItemControls($strEntity,&$formDef,$arrayValues,$strIndex)
		{
			$arrayErrors = array();
			$arrayEntity = $this->GetEntityDefinition($strEntity);			
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$bShowReadOnlyInModify = GetBoolValue(ArrayGetValue($arrayEntity,"showreadonlyinmodify"));
			$arrayReadOnlyValues = array();
			if ($strIndex == "")
			{
				$bShowReadOnlyInModify = false;
			}
			if ($bShowReadOnlyInModify == true)
			{
				$arrayReadOnlyValues = $arrayValues;
				$this->OnPrepareItemDataView($strEntity,$strIndex,$arrayReadOnlyValues);
			}
			$this->OnPrepareItemDataModify($strEntity,$strIndex,$arrayValues);
			
			$nTextAreaLinesDefault = 12;
			$nTextAreaLinesEntity = intval(ArrayGetValue($arrayEntity,"textarealines"));
			if ($nTextAreaLinesEntity > 0)
			{
				$nTextAreaLinesDefault = $nTextAreaLinesEntity;
			}
			
			$arrayContextFilters = $this->GetContextFilters($strEntity);
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$strModuleVarName = $this->GetModuleVarName($strAttribute);
				$strValue = ArrayGetValue($arrayValues,$strAttribute);
									
				if (ArrayKeyExists($arrayContextFilters,$strAttribute) == true)
				{
					$formDef->SetHiddenValue(
						$strModuleVarName,
						$strValue);
					continue;	
				}	
				
				$bReadOnly = $this->IsAttributeReadOnly($strEntity,$strAttribute);
				if (!$bReadOnly)
				{				
					$strType = ArrayGetValue($arrayAttribute,"type");
					$strControl = ArrayGetValue($arrayAttribute,"control");
					$strFormLabel = ArrayGetValue($arrayAttribute,"formlabel");
					if ($strFormLabel == "")
					{
						$strFormLabel = "?".$this->MakeEntityTID($strEntity,"LABEL",$strAttribute)."?";
					}
					if ($strControl == "")
					{
						if ($strType == "enum")
						{
							$strControl = "select";
						}
						else if ($strType == "language")
						{
							$strControl = "select";
						}	
						else if ($strType == "currency")
						{
							$strControl = "select";
						}							
					}
					switch($strControl)
					{
						case "select":
						{
							$arrayOptions = $this->OnPrepareSelectOptions(
								$strEntity,
								$strAttribute,
								$strIndex);
							$nOptionCount = ArrayCount($arrayOptions);
							if ($nOptionCount == 0)
							{
								$arrayErrors[$strAttribute] = "NOOPTIONSAVAILABLE";
								break;
							}
							if ($nOptionCount == 1)
							{
								$bHideNoOption = GetBoolValue(ArrayGetValue($arrayAttribute,"hidenooption"));
								if ($bHideNoOption == true)
								{
									$formDef->SetHiddenValue(
										$strModuleVarName,
										ArrayGetValueByPosition($arrayOptions,0));
									break;
								}
							}
							$formDef->AddControlSelect(
								StringUpperCase($strAttribute),
								$strModuleVarName,
								$strFormLabel,
								$arrayOptions,
								$strValue,
								false);
							break;
						}
						case "textarea":
						{
							$nMaxLen = intval(ArrayGetValue($arrayAttribute,"maxlen"));
							$nTextAreaLinesAttribute = intval(ArrayGetValue($arrayAttribute,"textarealines"));
							if ($nTextAreaLinesAttribute > 0)
							{
								$nTextAreaLines = $nTextAreaLinesAttribute;
							}
							else
							{
								$nTextAreaLines = $nTextAreaLinesDefault;
							}
							$formDef->AddControlTextArea(
								StringUpperCase($strAttribute),
								$strModuleVarName,
								$strFormLabel,
								$strValue,
								65,
								$nTextAreaLines, 
								false);	
							break;
						}
						default:
						{
							switch ($strType)
							{
								case "string":
								{
									$nMaxLen = intval(ArrayGetValue($arrayAttribute,"maxlen"));
									$formDef->AddControlText(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$strValue,
										40,
										$nMaxLen,
										false);
									break;	
								}
								case "email":
								{
									$formDef->AddControlText(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$strValue,
										40,
										EMAIL_MAXLEN,
										false);
									break;	
								}
								case "integer":
								{
									$nMax = intval(ArrayGetValue($arrayAttribute,"max"));
									if ($nMax == 0)
									{
										$this->Error("max value MUST be specified!");
									}
									$strMax = strval($nMax);
									//$nMin = intval(ArrayGetValue($arrayAttribute,"min"));
									
									$bZeroUnset = GetBoolValue(ArrayGetValue($arrayAttribute,"zerounset"));
									if ($bZeroUnset == true)
									{
										if ($strValue == 0)	
										{
											$strValue = "";
										}
									}
									$formDef->AddControlText(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$strValue,
										StringLength($strMax),
										StringLength($strMax),
										false);
									break;
								}
								case "numericid":
								{
									$bZeroUnset = GetBoolValue(ArrayGetValue($arrayAttribute,"zerounset"));
									if ($bZeroUnset == true)
									{
										if ($strValue == 0)	
										{
											$strValue = "";
										}
									}
									$formDef->AddControlText(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$strValue,
										StringLength(strval(PHP_INT_MAX)),
										StringLength(strval(PHP_INT_MAX)),
										false);
									break;
								}

								case "float":
								{
									$nPrecision = intval(ArrayGetValue($arrayAttribute,"floatprecision"));
									if ($nPrecision == 0)
									{
										$nPrecision = ENTITY_FLOATPRECISION_DEFAULT;
									}
									$fMax = floatval(ArrayGetValue($arrayAttribute,"max"));
									$fMin = floatval(ArrayGetValue($arrayAttribute,"min"));
									$strLengthMax = StringLength(strval(intval($fMax)));
									$strLengthMin = StringLength(strval(intval($fMin)));
									
									$nLength = max($strLengthMax,$strLengthMin);
									$nLength += $nLength/3; // consider thousand separators
									$nLength ++; // decimal marker 
									$nLength += $nPrecision; 
									
									$bZeroUnset = GetBoolValue(ArrayGetValue($arrayAttribute,"zerounset"));
									if ($bZeroUnset == true)
									{
										if ($strValue == 0)	
										{
											$strValue = "";
										}
									}
									$formDef->AddControlText(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$strValue,
										$nLength,
										$nLength,
										false);
									break;
								}						
								case "bool":
								{
									$bValue = GetBoolValue($strValue);
									$formDef->AddControlCheckbox(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$bValue);
									break;
								}
								case "password":
								{
									//$nMaxLen = intval(ArrayGetValue($arrayAttribute,"maxlen"));
									$formDef->AddControlPassword(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$strValue,
										40,
										255,
										false);
									break;	
								}
								case "date":
								{
									$this->Trace("AddControlDate:$strAttribute: $strValue");
									$formDef->AddControlDate(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$strValue,
										false);
									break;	
								}
								case "datetime":
								{
									$formDef->AddControlDateTime(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$strValue,
										false);
									break;	
								}
								case "sortorder":
								{
									break;	
								}
								case "language":
								{
									$arrayOptions = $this->OnPrepareSelectOptions(
										$strEntity,
										$strAttribute,
										$strIndex);
									if (ArrayCount($arrayOptions) == 0)
									{
										$arrayErrors[$strAttribute] = "NOOPTIONSAVAILABLE";
										break;
									}
									$strValue = StringLowerCase($strValue);
									$formDef->AddControlSelect(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$arrayOptions,
										$strValue,
										false);
									break;
								}
								case "language":
								{
									$arrayOptions = $this->OnPrepareSelectOptions(
										$strEntity,
										$strAttribute,
										$strIndex);
									if (ArrayCount($arrayOptions) == 0)
									{
										$arrayErrors[$strAttribute] = "NOOPTIONSAVAILABLE";
										break;
									}
									$strValue = StringLowerCase($strValue);
									$formDef->AddControlSelect(
										StringUpperCase($strAttribute),
										$strModuleVarName,
										$strFormLabel,
										$arrayOptions,
										$strValue,
										false);
									break;
								}
								default:
								{
									$this->Error("AddItemControls: Attribute \"$strAttribute\" has unsupported type: \"$strType\"");	
								}
							}
						}
					}
				}
				else
				{
					if ($bShowReadOnlyInModify == true)
					{
						$bHideInView = $this->IsAttributeHiddenInView($strEntity,$strAttribute);
						if (!$bHideInView)
						{
							$strValue = ArrayGetValue($arrayReadOnlyValues,$strAttribute);
							$strType = ArrayGetValue($arrayAttribute,"type");
							switch ($strType)
							{
							case "bool":
								break;
							default:
								break;	
							}
							$strFormLabel = ArrayGetValue($arrayAttribute,"formlabel");
							if ($strFormLabel == "")
							{
								$strFormLabel = "?".$this->MakeEntityTID($strEntity,"LABEL",$strAttribute)."?";
							}
							$formDef->AddControlReadOnly(
								StringUpperCase($strAttribute),
								$strFormLabel,
								$strValue);
						}
					}
				
				}
			}
			return $arrayErrors;
		}
		
		final
		function GetItemValuesFromWebservice($strEntity,$webservice)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$strEntityTag = ArrayGetValue($arrayEntity,"webservice","itemtag");
			$arrayValues = array();
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$strTag = StringUpperCase($strAttribute);
				if ($strTag != "")
				{
					$arrayValues[$strAttribute] = $webservice->GetResultValue($strEntityTag,$strTag);
				}
			}
			return $arrayValues;					
		}
		
		final
		function GetItemListFromWebservice($strEntity,$webservice)
		{
			$arrayEntity = $this->GetEntityDefinition($strEntity);			
			$arrayAttributes = ArrayGetValue($arrayEntity,"attributes");
			$strIndexAttribute = ArrayGetValue($arrayEntity,"indexattribute");
			
			$arrayResultList = $webservice->GetResultList();
			
			$arrayItemList = array();
			foreach ($arrayResultList as $arrayResultItem)
			{
				$arrayItem = array();
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					$strTag = StringUpperCase($strAttribute);
					$arrayItem[$strAttribute] = ArrayGetValue($arrayResultItem,$strTag);
					if ($strAttribute == $strIndexAttribute)
					{
						$arrayItem["INDEX"] = $arrayItem[$strAttribute];
					}
					
				}
				array_push($arrayItemList,$arrayItem);
			}
			return $arrayItemList;					
		}
		
		function GetEntity($strEntity,$strIndex,&$arrayItem,&$strError)
		{
			$arrayItem = array();
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			
			$arrayParams = array();
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strIndex;
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"get");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				return false;
			}
			$arrayItem = $this->GetItemValuesFromWebservice($strEntity,$webservice);
			return true;			
		}

		
		function MakeEntityTID($strEntity,$strType,$strID)
		{
			if ($strEntity != "")
			{
				$strEntity .= "_";
			}
			return "TID_".StringUpperCase($strEntity).StringUpperCase($strType)."_".StringUpperCase($strID);
		}
		
		function LoadStringEntity($strEntity,$strType,$strID)
		{
			$strString = $this->LoadString($this->MakeEntityTID($strEntity,$strType,$strID));
			if ($strString == "")
			{
				$strString = $this->LoadString($this->MakeEntityTID("",$strType,$strID));
			}
			return $strString;
		}
		
		function AddErrorMessageCodeEntity($strEntity,$strErrorMessageCode)
		{
			$this->GetWebsite()->AddErrorMessageCode($strErrorMessageCode,$strEntity);
		}
		function AddWarningMessageCodeEntity($strEntity,$strWarningMessageCode)
		{
			$this->GetWebsite()->AddWarningMessageCode($strWarningMessageCode,$strEntity);
		}
		function AddInfoMessageCodeEntity($strEntity,$strInfoMessageCode)
		{
			$this->GetWebsite()->AddInfoMessageCode($strInfoMessageCode,$strEntity);
		}

		function SetErrorMessageCodeEntity($strEntity,$strErrorMessageCode)
		{
			$this->GetWebsite()->SetErrorMessageCode($strErrorMessageCode,$strEntity);
		}
		function SetWarningMessageCodeEntity($strEntity,$strWarningMessageCode)
		{
			$this->GetWebsite()->SetWarningMessageCode($strWarningMessageCode,$strEntity);
		}
		function SetInfoMessageCodeEntity($strEntity,$strInfoMessageCode)
		{
			$this->GetWebsite()->SetInfoMessageCode($strInfoMessageCode,$strEntity);
		}
		
		
		function IsAttributeReadOnly($strEntity,$strAttribute)
		{
			$arrayAttribute = $this->GetEntityAttributeDefinition($strEntity,$strAttribute);
			if (!is_array($arrayAttribute))
			{
				return false;	
			}
			if (GetBoolValue(ArrayGetValue($arrayAttribute,"readonly")) == true)
			{
				return true;
			}
			if (GetBoolValue(ArrayGetValue($arrayAttribute,"readonlyfrontend")) == true)
			{
				return true;
			}
			if (is_array(ArrayGetValue($arrayAttribute,"foreignkey")) == true)
			{
				return true;
			}
			return false;
		}

		function IsAttributeInContextFilters($strEntity,$strAttribute)
		{
			$arrayContextFilters = $this->GetContextFilters($strEntity);
			if (ArrayKeyExists($arrayContextFilters,$strAttribute))
			{
				return true;	
			}
			$arrayAttribute = $this->GetEntityAttributeDefinition($strEntity,$strAttribute);
			if (!is_array($arrayAttribute))
			{
				return false;	
			}
			$strForeignAttribute = ArrayGetValue($arrayAttribute,"foreignkey","attribute");
			if (ArrayKeyExists($arrayContextFilters,$strForeignAttribute) == true)
			{
				return true;	
			}
			return false;
		}

		
		function IsAttributeHidden($strEntity,$strAttribute)
		{
			if ($this->IsAttributeInContextFilters($strEntity,$strAttribute) == true)
			{
				return true;	
			}
/*
			$arrayAttribute = $this->GetEntityAttributeDefinition($strEntity,$strAttribute);
			if (!is_array($arrayAttribute))
			{
				return false;	
			}
			*/
			return false;
		}
		
		function IsAttributeHiddenInView($strEntity,$strAttribute)
		{
			if ($this->IsAttributeHidden($strEntity,$strAttribute) == true)
			{
				return true;	
			}
			$arrayAttribute = $this->GetEntityAttributeDefinition($strEntity,$strAttribute);
			if (!is_array($arrayAttribute))
			{
				return false;	
			}
			return GetBoolValue(ArrayGetValue($arrayAttribute,"hideinview"));
		}
		
		function IsAttributeHiddenInList($strEntity,$strAttribute)
		{
			if ($this->IsAttributeHidden($strEntity,$strAttribute) == true)
			{
				return true;	
			}
			$arrayAttribute = $this->GetEntityAttributeDefinition($strEntity,$strAttribute);
			if (!is_array($arrayAttribute))
			{
				return false;	
			}
			return GetBoolValue(ArrayGetValue($arrayAttribute,"hideinlist"));
		}
		
		function MakeDefaultTIDPattern($strEntity,$strAttribute)
		{
			return "?TID_".StringUpperCase($strEntity)."_".StringUpperCase($strAttribute)."_{VALUE}?";
		}
		
		function GetValueTID($strEntity,$strAttribute,$strValue,$bUseDefault = false)
		{
			if ($strValue == "")
			{
				return "";	
			}
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			$arrayAttribute = ArrayGetValue($arrayEntity,"attributes",$strAttribute);
			if (!is_array($arrayAttribute))
			{
				return "";
			}
			$strTIDPattern = ArrayGetValue($arrayAttribute,"tidpattern");
			if ($strTIDPattern == "")
			{
				if ($bUseDefault == true)
				{
					$strTIDPattern = $this->MakeDefaultTIDPattern($strEntity,$strAttribute);	
				}
			}
			$strTID = ReplaceString($strTIDPattern,"{VALUE}",StringUpperCase($strValue));
			return $strTID;
		}	
		
		function GetSupportedLanguages()
		{
			$arrayLanguages = $this->GetModuleConfig("languages");
			if (is_array($arrayLanguages) == false)
			{
				$arrayLanguages = $this->GetWebsite()->GetSupportedLanguages();	
			}
			return $arrayLanguages;			
		}
		
	}

