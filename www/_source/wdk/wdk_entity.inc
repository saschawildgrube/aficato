<?php

	require_once(GetWDKDir()."wdk_entitydefinitions.inc");

	class CEntity
	{
		private $m_webapp;
		private $m_strEntity;
		private $m_entityDef;
		private $m_arrayItem;
		
		function __construct(CWebApplication $webapp, $strEntity, $arrayEntityDef)
		{
			$this->m_arrayItem = array();
			if (!is_subclass_of($webapp,"CWebApplication"))
			{
				FatalError("CEntity has been initialized with an invalid webapp object.");
			}
			$this->m_webapp = $webapp;
			$this->m_entityDef = new CEntityDefinitions();
			$this->SetEntityDefinition($strEntity,$arrayEntityDef);
		}
		
		function GetWebApp()
		{
			return $this->m_webapp;	
		}

		function SetEntityDefinition($strEntity,$arrayEntityDef)
		{
			if (!is_array($arrayEntityDef))
			{
				$this->m_arrayData = array();
				return false;	
			}
			$this->m_strEntity = $strEntity;
			$arrayEntityDefs = array();
			$arrayEntityDefs[$strEntity] = $arrayEntityDef;
			$this->m_entityDef->SetEntityDefinitions($arrayEntityDefs);
			return true;
		} 
	
		function GetEntityDefinition()
		{
			return $this->m_entityDef->GetEntityDefinition($this->m_strEntity);	
		}
		
		function GetEntityName()
		{
			return $this->m_strEntity;	
		}
		function GetFieldName($strAtribute)
		{
			return $this->m_entityDef->GetFieldName($this->m_strEntity,$strAtribute);
		}
		
		function SetItemData($arrayItem)
		{
			$this->m_arrayItem = $arrayItem;	
		}
		
		function SetItemDataFromWebserviceResult($arrayItemWebservice)
		{
			$arrayAttributes = $this->GetAttributes();
			$arrayItem = array();
			foreach ($arrayAttributes as $strID => $arrayAttribute)
			{
				$strTag = StringUpperCase($strID);
				if ($strTag != "")
				{
					$arrayItem[$strID] = ArrayGetValue($arrayItemWebservice,$strTag);
				}
			}
			$this->SetItemData($arrayItem);
			
		}
		
		function GetItemData()
		{
			return $this->m_arrayItem;
		}
		
		function GetItemDataIndex()
		{
			return ArrayGetValue($this->GetItemData(),$this->GetIndexAttribute());
		}
		
		function GetAttributes()
		{
			$arrayAttributes = ArrayGetValue($this->GetEntityDefinition(),"attributes");
			if (!is_array($arrayAttributes))
			{
				return false;
			}
			return $arrayAttributes;
		}
		
		function GetIndexAttribute()
		{
			return ArrayGetValue($this->GetEntityDefinition(),"indexattribute");
		}
		
		function GetBlockSize()
		{
			return ArrayGetValue($this->GetEntityDefinition(),"blocksize");
		}
		function GetMaxBlockSize()
		{
			return ArrayGetValue($this->GetEntityDefinition(),"maxblocksize");
		}				
		
		function GetDatabaseTableName()
		{
			return ArrayGetValue($this->GetEntityDefinition(),"dbtablename");
		}
		
		function MakeSQL_CreateTable()
		{
			
			/*
			  FULLTEXT KEY `PRODUCT_NAME` (`PRODUCT_NAME`)
			*/
			
		
			$strSQL = "CREATE TABLE ".MakeSQL_Helper_Table($this->GetDatabaseTableName())." (\n";
			
			$arrayAttributes = $this->GetAttributes();
			$strIndexAttribute = $this->GetIndexAttribute();
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bNoStorage = ArrayGetValue($arrayAttribute,"nostorage");
				if ($bNoStorage == true)
				{
					continue;	
				}
				$strType = ArrayGetValue($arrayAttribute,"type");
				$strDBType = ArrayGetValue($arrayAttribute,"dbtype");
				$strDefault = ArrayGetValue($arrayAttribute,"default");					
				$bFilterOption = GetBoolValue(ArrayGetValue($arrayAttribute,"filteroption"));
				$bSortOption = GetBoolValue(ArrayGetValue($arrayAttribute,"sortoption"));
				$strDBFieldName = $this->m_entityDef->GetFieldName($this->m_strEntity,$strAttribute);

				switch ($strType)
				{
				case "numericid":
					if ($strIndexAttribute == $strAttribute)
					{
						$strAutoInc = " auto_increment";
					}
					else
					{
						$strAutoInc = "";
					}
					$strSQL .= "\t`".$strDBFieldName."` bigint(20) unsigned NOT NULL".$strAutoInc.",\n";
					break;
				
				case "string":
					$nMaxLen = intval(ArrayGetValue($arrayAttribute,"maxlen"));
					if ($nMaxLen == 0)
					{
						$nMaxLen = 255;
					}
					if ($nMaxLen <= 255)
					{
						$strSQL .= "\t`".$strDBFieldName."` varchar(".strval($nMaxLen).") NOT NULL,\n"; // default ''
						if ($bFilterOption || $bSortOption)
						{
							$strSQL .= "\tKEY `".$strDBFieldName."_INDEX` (`".$strDBFieldName."`),\n";	
						}
					}
					else
					{
						$strSQL .= "\t`".$strDBFieldName."` longtext NOT NULL,\n";
					}
					break;
				
				case "bool":
					$strSQL .= "\t`".$strDBFieldName."` tinyint(1) NOT NULL default ".RenderBool($strDefault).",\n";
					break;
					
				case "float":
					$nMax = intval(floatval(ArrayGetValue($arrayAttribute,"max")));
					$nPrecision = intval(ArrayGetValue($arrayAttribute,"floatprecision"));
					if ($nPrecision == 0)
					{
						$nPrecision = ENTITY_FLOATPRECISION_DEFAULT;	
					}
					$nMysqlScale = $nPrecision;
					$nMysqlPrecision = StringLength(strval($nMax)) + $nPrecision;
					
					$strSQL .= "\t`".$strDBFieldName."` decimal(".$nMysqlPrecision.",".$nMysqlScale.") default NULL,\n";
					break;
					
				case "integer":
					$nMax = intval(ArrayGetValue($arrayAttribute,"max"));
					$strSQL .= "\t`".$strDBFieldName."` bigint(20) unsigned NOT NULL,\n";
					break;					

				case "date":
					$strSQL .= "\t`".$strDBFieldName."` date NOT NULL,\n";
					if ($bFilterOption || $bSortOption)
					{
						$strSQL .= "\tKEY `".$strDBFieldName."_INDEX` (`".$strDBFieldName."`),\n";	
					}
					break;
				
				case "datetime":
					$strSQL .= "\t`".$strDBFieldName."` datetime NOT NULL,\n";
					if ($bFilterOption || $bSortOption)
					{
						$strSQL .= "\tKEY `".$strDBFieldName."_INDEX` (`".$strDBFieldName."`),\n";	
					}
					break;
					
				case "enum":
					$arrayEnum = ArrayGetValue($arrayAttribute,"enum");
					if (!is_array($arrayEnum))
					{
						return false;	
					}
					switch ($strDBType)
					{
					case "string":
						$nMaxLen = ArrayMaxStringLength($arrayEnum);
						if ($nMaxLen > 255)
						{
							return false;	
						}
						$strSQL .= "\t`".$strDBFieldName."` varchar(".strval($nMaxLen).") NOT NULL default ".MakeSQL_Helper_Value($strDefault).",\n";
						break;

					default: // "enum"
						$strSQL .= "\t`".$strDBFieldName."` enum(";
						$strEnumOptions = "";
						foreach ($arrayEnum as $strValue)
						{
							if ($strEnumOptions != "")
							{
								$strEnumOptions .= ",";
							}
							$strEnumOptions .= MakeSQL_Helper_Value(StringUpperCase($strValue));
							//$strEnumOptions .= MakeSQL_Helper_Value($strValue);
						}
						$strSQL .= $strEnumOptions;
						$strSQL .= ") NOT NULL default ".MakeSQL_Helper_Value(StringUpperCase($strDefault)).",\n";
						if ($bFilterOption || $bSortOption)
						{
							$strSQL .= "\tKEY `".$strDBFieldName."_INDEX` (`".$strDBFieldName."`),\n";	
						}												
						break;
					}
					break;

				case "sortorder":
					$strAutoInc = " auto_increment";
					$strSQL .= "\t`".$strDBFieldName."` bigint(20) unsigned NOT NULL,\n";
					/*
					if ($bFilterOption || $bSortOption)
					{
						$strSQL .= "\tKEY `".$strDBFieldName."_INDEX` (`".$strDBFieldName."`),\n";	
					}
					*/
					break;

				case "language":
					$strSQL .= "\t`".$strDBFieldName."` varchar(2) NOT NULL,\n"; // default ''
					if ($bFilterOption || $bSortOption)
					{
						$strSQL .= "\tKEY `".$strDBFieldName."_INDEX` (`".$strDBFieldName."`),\n";	
					}
					break;

				case "currency":
					$strSQL .= "\t`".$strDBFieldName."` varchar(3) NOT NULL,\n"; // default ''
					if ($bFilterOption || $bSortOption)
					{
						$strSQL .= "\tKEY `".$strDBFieldName."_INDEX` (`".$strDBFieldName."`),\n";	
					}
					break;

				case "email":
					$strSQL .= "\t`".$strDBFieldName."` varchar(".EMAIL_MAXLEN.") NOT NULL,\n"; // default ''
					if ($bFilterOption || $bSortOption)
					{
						$strSQL .= "\tKEY `".$strDBFieldName."_INDEX` (`".$strDBFieldName."`),\n";	
					}
					break;

					
				default:
					return false;
					break;	
				}
			}
			
			
			$arrayUniqueIndices = ArrayGetValue($this->GetEntityDefinition(),"uniqueindices");
			if (is_array($arrayUniqueIndices))
			{
				foreach ($arrayUniqueIndices as $strUniqueIndexName => $arrayUniqueIndex)	
				{
					$strSQL .= 
						"\tUNIQUE KEY `"
						. StringUpperCase($strUniqueIndexName)
						. "` (";
					$strUniqueIndexAttributes = "";
					foreach ($arrayUniqueIndex as $strUniqueIndexAttribute)
					{
						if ($strUniqueIndexAttributes != "")
						{
							$strUniqueIndexAttributes.=", ";
						}
						$strUniqueIndexAttributes.="`".$this->GetFieldName($strUniqueIndexAttribute)."`";
					}
					$strSQL .=
						$strUniqueIndexAttributes
						. "),\n";
					
				}
			}


			$strSQL .= 
				"\tPRIMARY KEY (`"
				. StringUpperCase($strIndexAttribute)
				. "`)\n";
			
			
			
			$strSQL .= ") ENGINE=MyISAM DEFAULT CHARSET=utf8;";
			
			
			return $strSQL;
		}
		
		function GetWebserviceCommand($strCommandName)
		{
			$arrayEntity = $this->GetEntityDefinition();
			$strCommand = ArrayGetValue($arrayEntity,"webservice","commands",$strCommandName,"name");
			if ($strCommand == "")
			{
				$bDefault = GetBoolValue(ArrayGetValue($arrayEntity,"default"));
				if ($strCommandName == "list")
				{
					$strCommand = ($bDefault)?("list"):($this->GetEntityName()."list");	
				}
				else
				{
					$strCommand = ($bDefault)?($strCommandName):($strCommandName.$this->GetEntityName());	
				}
			}
			return $strCommand;
		}
		
		final
		function GetIndexPropertyWebserviceParamName()
		{
			$arrayEntity = $this->GetEntityDefinition();
			$strIndexProperty = ArrayGetValue($arrayEntity,"indexattribute");
			return ArrayGetValue($arrayEntity,"attributes",$strIndexProperty,"webserviceparam");
		}
	
		function GetItemFromWebservice($strIndex,$arrayParams = array())
		{
			$arrayEntityDef = $this->GetEntityDefinition();
			$webservice = new CWebserviceConsumerWebApplication($this->GetWebApp());

			$arrayParams = array();
			$arrayParams["command"] = $this->GetWebserviceCommand("get");
			$arrayParams[$this->GetIndexPropertyWebserviceParamName()] = $strIndex;
			$webservice->ConsumeWebService(
				ArrayGetValue($arrayEntityDef,"webservice","name"),
				$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				return false;
			}
			$this->SetItemDataFromWebserviceResult(
				$webservice->GetResultValueInsecure(
					ArrayGetValue($arrayEntityDef,"webservice","itemtag")));
			return true;	
		}
		
		function GetFullListFromWebservice($arraySortOptions,$arrayFilterOptions,$arrayParams = array())
		{ 
			$arrayEntity = $this->GetEntityDefinition();
			
			$arrayList = array();
			
			$arrayAttributes = $this->GetAttributes();
			$strIndexAttribute = $this->GetIndexAttribute();
			$nMaxBlockSize = $this->GetMaxBlockSize();
			$bDefault = GetBoolValue(ArrayGetValue($arrayEntity,"default"));
			$strWebserviceParamPrefix = ($bDefault)?(""):($strEntity."_");
			
			// PREPARE WEBSERVICE
			$strWebservice = ArrayGetValue($this->GetEntityDefinition(),"webservice","name");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebApp());

			// BLOCKSIZE
			if ($nMaxBlockSize != 0)
			{
				$arrayParams["blocksize"] = $nMaxBlockSize;	
			}
			
			// SORT OPTIONS
			$nSortOption = 1;
			foreach ($arraySortOptions as $strSortOption => $strSortOrder)
			{
				$arrayParams[$strWebserviceParamPrefix."sort".$nSortOption] = $strSortOption;
				$arrayParams[$strWebserviceParamPrefix."sort".$nSortOption."_order"] = $strSortOrder;
				$nSortOption++;
			}

			// FILTER OPTIONS
			$nFilterOption = 1;
			foreach ($arrayFilterOptions as $arrayFilterOption)
			{
				$strWebserviceFilterParam = $strWebserviceParamPrefix."filter";
				$arrayParams[$strWebserviceFilterParam.$nFilterOption] = ArrayGetValue($arrayFilterOption,"field");
				$arrayParams[$strWebserviceFilterParam.$nFilterOption."_operator"] = ArrayGetValue($arrayFilterOption,"operator");
				$arrayParams[$strWebserviceFilterParam.$nFilterOption."_value"] = ArrayGetValue($arrayFilterOption,"value");;
				$nFilterOption++;
			}
				
			$nOffset = 0;
			$arrayListRaw = array();
			do
			{
				$arrayParams["offset"] = $nOffset;
		
				// INVOKE WEBSERVICE
				$arrayParams["command"] = "list";
				$webservice->ConsumeWebService($strWebservice,$arrayParams);
				$strError = $webservice->GetError();
				if ($strError != "")
				{
					//$this->SetErrorMessageCode($strError);
					return false;
				}

				// GET MAXOFFSET
				$nMaxOffset = intval($webservice->GetResultValue("MAXOFFSET"));
						
				// ADD ITEM DATA
				$arrayListRaw = ArrayAppend($arrayListRaw,$webservice->GetResultListInsecure());
				
				$nOffset++;
				
			} while ($nOffset-1 < $nMaxOffset);
			
			foreach ($arrayListRaw as $arrayItemRaw)
			{
				$this->SetItemDataFromWebserviceResult($arrayItemRaw);
				$arrayList[] = $this->GetItemData();
			}
			
			return $arrayList;
		}
		

		
	}



