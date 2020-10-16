<?php


	function GetVersionInfoMySQL()
	{
		$arrayVersion = array();
		$arrayVersion["NAME"] = "MySQL";
		
		$arrayResult = array();
		$bResult = ShellExecute(
			"mysql",
			array("--version"),
			array(),
			"",
			$arrayResult);

		if ($bResult == false)
		{
			return $arrayVersion;		
		}
		
		$strStdout = ArrayGetValue($arrayResult,"stdout");
		$arrayVersion["VERBOSE"] = $strStdout;
		
		$nPos = FindString($strStdout,"Distrib ");
		if ($nPos == -1)
		{
			return $arrayVersion;
		}
		
		$strVersion = StringSection($strStdout,$nPos+8);
		$nPos = FindString($strVersion,",");
		if ($nPos == -1)
		{
			return $arrayVersion;	
		}
		
		$strVersion = StringSection($strVersion,0,$nPos);
		$arrayVersion["VERSION"] = $strVersion;

		return $arrayVersion;
	}


	class CDatabase
	{
		private $m_nDBLink;
		private $m_strTableNamePrefix;
		private $m_result;
		
		private $m_arrayData;

		function __construct()
		{
			$this->m_arrayData = array();
			$this->m_nDBLink = 0;
			$this->m_strTableNamePrefix = "";
			$this->m_result = false;
		}
		
		
		function Connect(
			$strDBServer,
			$strDBUser,
			$strDBPassword,
			$strDBName,
			$strTableNamePrefix = "",
			$strDBPort = ""
			)
		{
			$nDBPort = intval($strDBPort);	
			if ($nDBPort == 0)
			{
				$nDBPort = intval(ini_get("mysqli.default_port"));
			}
			$ea = new CErrorAmnesty();
			$ea->Start();
			$nDBLink = mysqli_connect(
				$strDBServer,
				$strDBUser,
				$strDBPassword,
				$strDBName,
				$nDBPort); 
			$ea->End();
			if ($nDBLink == false)
			{
				return false;	
			}
			$this->m_nDBLink = $nDBLink;
			$this->m_strTableNamePrefix = $strTableNamePrefix;
			$this->Query("SET NAMES 'utf8'"); 
			return true;		
		}
		
		function IsConnected()
		{
			if ($this->m_nDBLink != 0)
			{
				return true;
			}
			return false;
		}
		
		function Disconnect()
		{
			if ($this->m_nDBLink != 0)
			{
				mysqli_close($this->m_nDBLink);
			}
			
			$this->m_nDBLink = 0;	
			
		}
		
		function GetTableNamePrefix()
		{
			return $this->m_strTableNamePrefix;	
		}
		
		function ResourceToArray()
		{
			$this->m_arrayData = array();
			if ($this->m_result == false)	
			{
				return false;
			}
			if (is_object($this->m_result))
			{
				$nNumRows = mysqli_num_rows($this->m_result);
				for ($nIndex = 0; $nIndex < $nNumRows; $nIndex++)
				{
					array_push($this->m_arrayData,mysqli_fetch_array($this->m_result,MYSQLI_ASSOC)); 
				}
			}
			return true;
		}
		
		
		
		function Query($strSQL)
		{
			$strSQL = MakeSQL_PrefixTables(
				$strSQL,
				$this->GetTableNamePrefix());
				
			if ($strSQL == "")
			{
				return false;	
			}

			$this->m_result = mysqli_query($this->m_nDBLink,$strSQL);
			
			if ($this->GetErrorCode() != 0)
			{
				return false;
			}
			
			$this->ResourceToArray();
			return true;
		}
		
		
		function missing_mysqli_result($res, $row, $field=0)
		{ 
  	  $res->data_seek($row); 
    	$datarow = $res->fetch_array(); 
			return $datarow[$field]; 
		}	 

		
		function QueryFoundRows()
		{
			$result = mysqli_query($this->m_nDBLink,"SELECT FOUND_ROWS()");
			if ($result == false)
			{
				return -1;
			}

			$nTotalRows = $this->missing_mysqli_result($result, 0);
				
			return $nTotalRows;
		}
		
		
		
		
		function GetAffectedRows()
		{
			return mysqli_affected_rows($this->m_nDBLink);	
		}
		
		function GetLastInsertID()
		{
			return mysqli_insert_id($this->m_nDBLink);
		}
		
		function GetErrorText()
		{
			return mysqli_error($this->m_nDBLink);	
		}
		
		function GetErrorCode()
		{
			return mysqli_errno($this->m_nDBLink);	
		}		
		
		function GetData()
		{
			return $this->m_arrayData;	
		}
	
  	function TransactionBegin()
  	{ 
  		$result = mysqli_query($this->m_nDBLink,"BEGIN");
  		if ($result == false)
			{
				return false;
			}
			return true;  			
  	}
  	
  	function TransactionRollback()
  	{ 
   		$result = mysqli_query($this->m_nDBLink,"ROLLBACK"); 
  		if ($result == false)
			{
				return false;
			}
			return true;  			
		}

		function TransactionCommit()
		{ 
  		$result = mysqli_query($this->m_nDBLink,"COMMIT"); 
  		if ($result == false)
			{
				return false;
			}
			return true;  			
		}
		
		function LockTable($strTable)
		{
			$strSQL = "LOCK TABLES ".MakeSQL_Helper_Table($strTable)." WRITE;";

			$strSQL = MakeSQL_PrefixTables(
			$strSQL,
			$this->GetTableNamePrefix());
			
			$result = mysqli_query($this->m_nDBLink,$strSQL); 
  		if ($result == false)
			{
				return false;
			}
			return true;  			
		}

		function UnlockTables()
		{
			$result = mysqli_query($this->m_nDBLink,"UNLOCK TABLES;"); 
  		if ($result == false)
			{
				return false;
			}
			return true;  			
		}
	}

	function MakeSQL_Helper_ConditionalExpression($vCondition,&$strError = null,$bRoot = true)
	{
		if (is_string($vCondition))
		{
			if ($bRoot == true)
			{
				return $vCondition;
			}
			return MakeSQL_Helper_Value($vCondition);
		}
		if (is_numeric($vCondition))
		{
			return MakeSQL_Helper_Value($vCondition);
		}

		if (!is_array($vCondition))
		{
			$strError = "vCondition is not an array.";
			return false;
		}
		if (ArrayCount($vCondition)==0)
		{
			$strError = "vCondition is an empty array.";
			return false;	
		}
		$strSQL = ArrayGetValue($vCondition,"sql");
		if ($strSQL != "")
		{
			return $strSQL;
		}
		$strField = ArrayGetValue($vCondition,"field");
		$strTable = ArrayGetValue($vCondition,"table");
		$vLeft = ArrayGetValue($vCondition,"left");
		$vRight = ArrayGetValue($vCondition,"right");
		$strOperator = ArrayGetValue($vCondition,"operator");
		$arrayAnd = ArrayGetValue($vCondition,"and");
		$arrayValues = ArrayGetValue($vCondition,"values");
		$strSQL = "";
		if (is_array($arrayAnd))
		{
			$strSQL = "";
			foreach ($arrayAnd as $vCondition)
			{
				if ($strSQL != "")
				{
					$strSQL .= " AND ";
				}
				$strSQL2 = MakeSQL_Helper_ConditionalExpression($vCondition,$strError,false);
				if ($strSQL2 == false)
				{
					$strError = "MakeSQL_Helper_ConditionalExpression(vCondition[and][]) returned false. ".$strError;
					return false;
				}
				$strSQL .= $strSQL2;
			}
			return $strSQL;
		}
		if ($strField != "")
		{
			if ($vLeft != NULL || $vRight != NULL || $strOperator != "" || $arrayValues != NULL)
			{
				$strError = "If vCondition[field] is set vCondition[left], vCondition[right], vCondition[values] and vCondition[operator] must be empty.";
				return false;	
			}
			$strSQL .= MakeSQL_Helper_Field($strField,$strTable);
			return $strSQL;
		}
		if ($vLeft != NULL)
		{
			if ($vRight === NULL || $strOperator == "")
			{
				$strError = "If vCondition[left] is set vCondition[right] and vCondition[operator] must also be set.";
				return false;	
			}
			$arrayOperators = array("=",">=",">","<=","<","<>","!=","OR","||","XOR","AND","&&","NOT","IN","LIKE");
			if (ArrayValueExists($arrayOperators,$strOperator)==false)
			{
				$strError = "Operator $strOperator is not supported.";
				return false;
			}
  
			
			$strSQL = "(";
			$strLeft = MakeSQL_Helper_ConditionalExpression($vLeft,$strError,false);
			if ($strLeft === false)
			{
				$strError = "MakeSQL_Helper_ConditionalExpression(vCondition[left]) returned false. ".$strError;
				return false;	
			}
			$strSQL .= $strLeft;
			$strSQL .= " ".$strOperator." ";
			$strRight = MakeSQL_Helper_ConditionalExpression($vRight,$strError,false);
			if ($strRight === false)
			{
				$strError = "MakeSQL_Helper_ConditionalExpression(vCondition[right]) returned false. ".$strError;
				return false;	
			}
			$strSQL .= $strRight.")";
			return $strSQL;
		}
		if ($arrayValues != NULL)
		{
			$strSQLValues = MakeSQL_Helper_Assignments($arrayValues,$strError);
			if ($strSQLValues === false)
			{
				$strError = "MakeSQL_Helper_Assignments(vCondition[values]) returned false. ".$strError;
				return false;
			}
			return "(".$strSQLValues.")";
		}
		$strError = "Neither vCondition[and] nor vCondition[field] nor vCondition[left] nor vCondition[values] is set.";
		return false;
	}

	function MakeSQL($arraySQL,&$strError = null)
	{
		if (!is_array($arraySQL))
		{
			$strError = "Parameter arraySQL is not an array.";
			return false;	
		}
		if (ArrayCount($arraySQL)==0)
		{
			$strError = "Parameter arraySQL is an empty array.";
			return false;	
		}
		$arrayAllowedKeys = array("command","tables","where","groupby","limit","orderby","calculate_found_rows","fields","values","on_duplicate_key_update");   
		$arrayForbiddenKeys = array();
		if (ArrayCheckKeysByWhiteList($arraySQL,$arrayAllowedKeys,$arrayForbiddenKeys) == false)
		{
			$strError = "Parameter arraySQL contains forbidden keys: ".ArrayRenderList($arrayForbiddenKeys);
			return false;	
		}
		
		$strSQL = "";
		$strCommand = ArrayGetValue($arraySQL,"command");
		$arrayCommands = array("SELECT","INSERT","UPDATE","REPLACE","DELETE");
		if (ArrayValueExists($arrayCommands,$strCommand) == false)
		{
			$strError = "Specified arraySQL[command] \"$strCommand\" is not supported.";
			return false;	
		}
		
		if ($strCommand == "SELECT")
		{
			$strSQL = "SELECT ";
			if (ArrayGetValue($arraySQL,"calculate_found_rows") == true)
			{
				$strSQL .= "SQL_CALC_FOUND_ROWS ";
			}
			$arrayFields = ArrayGetValue($arraySQL,"fields");
			$strSQL .= MakeSQL_Helper_Fields($arrayFields,$strError);
			if ($strError != "")
			{
				return false;	
			}

			$strSQL .= " FROM ";
			$vFrom = ArrayGetValue($arraySQL,"tables");	
			if (is_string($vFrom))
			{
				$strSQL .= MakeSQL_Helper_Table($vFrom);	
			}
			else
			{
				if (!is_array($vFrom))
				{
					$strError = "arraySQL[from] is neither string nor array.";
					return false;	
				}
				if (ArrayCount($vFrom) == 0)
				{
					$strError = "arraySQL[from] is an empty array.";
					return false;	
				}
				$bFirst = true;
				foreach ($vFrom as $arrayTableRef)
				{
					if (!is_array($arrayTableRef))	
					{
						$strError = "arraySQL[from][] is not an array.";
						return false;	
					}
					$strTable = ArrayGetValue($arrayTableRef,"table");
					if ($strTable == "")
					{
						$strError = "arraySQL[from][][table] must not be empty.";
						return false;	
					}
					$strJoin = ArrayGetValue($arrayTableRef,"join");
					if ($bFirst)
					{
						if ($strJoin != "")
						{
							$strError = "arraySQL[from][][join] must not be empty.";
							return false;	
						}
					}
					else
					{
						$arrayJoins = array("LEFT JOIN","JOIN","RIGHT JOIN",);
						if (ArrayValueExists($arrayJoins,$strJoin) == false)
						{
							$strError = "arraySQL[from][][join] = $strJoin is not supported.";
							return false;	
						}
						$strSQL .= $strJoin." ";
						
					}
					$strSQL .= MakeSQL_Helper_Table($strTable)." ";	
					$strAS = ArrayGetValue($arrayTableRef,"as");
					if ($strAS != "")
					{
						$strSQL .= "AS ".MakeSQL_Helper_Table($strAS)." ";	
					}
					$vOn = ArrayGetValue($arrayTableRef,"on");
					if ($bFirst)
					{
						if ($vOn != NULL)
						{
							$strError = "arraySQL[from][][on] must be null for the first table reference.";							
							return false;	
						}
					}
					else
					{
						if ($vOn != NULL)
						{
							$strConditionalExpression = MakeSQL_Helper_ConditionalExpression($vOn,$strError);
							if ($vOn == false)
							{
								$strError = "MakeSQL_Helper_ConditionalExpression(arraySQL[from][][on]) returned false. ".$strError;
								return false;	
							}
							$strSQL .= "ON ".$strConditionalExpression;
						}
					}
					$bFirst	= false;
				}
			}
			$vWhere = ArrayGetValue($arraySQL,"where");
			if ($vWhere != NULL)
			{
				$strWhereCondition = MakeSQL_Helper_ConditionalExpression($vWhere,$strError);
				if ($strWhereCondition == false)
				{
					$strError = "MakeSQL_Helper_ConditionalExpression(arraySQL[where]) returned false. ".$strError;
					return false;	
				}
				$strSQL .= " WHERE ".$strWhereCondition;
			}
			$arraySQLGroupBy = ArrayGetValue($arraySQL,"groupby");
			$strSQL .= MakeSQL_Helper_GroupBy($arraySQLGroupBy,$strError);
			if ($strError != "")
			{
				return false;	
			}
		
			$arrayOrderBy = ArrayGetValue($arraySQL,"orderby");
			$strSQL .= MakeSQL_Helper_OrderBy($arrayOrderBy);

			$arrayLimit = ArrayGetValue($arraySQL,"limit");
			$strSQL .= MakeSQL_Helper_Limit($arrayLimit);
			
			$strSQL .= ";";
			return $strSQL;
		}


		if ($strCommand == "UPDATE")
		{
			$strSQL = "UPDATE ";
						
			$vTable = ArrayGetValue($arraySQL,"tables");	
			if (is_string($vTable))
			{
				$strSQL .= MakeSQL_Helper_Table($vTable);	
			}
			else
			{
				$strError = "arraySQL[tables] is not a string.";
				return false;
			}
			
			$arrayValues = ArrayGetValue($arraySQL,"values");
			
			$strSQLValues = MakeSQL_Helper_Assignments($arrayValues,$strError);
			if ($strSQLValues == "")
			{
				return false;	
			}
			$strSQL .= " SET ".$strSQLValues; 

			$vWhere = ArrayGetValue($arraySQL,"where");
			if ($vWhere != NULL)
			{
				$strWhereCondition = MakeSQL_Helper_ConditionalExpression($vWhere,$strError);
				if ($strWhereCondition == false)
				{
					$strError = "MakeSQL_Helper_ConditionalExpression(arraySQL[where]) returned false. ".$strError;
					return false;	
				}
				$strSQL .= " WHERE ".$strWhereCondition;
			}
			
			$arrayOrderBy = ArrayGetValue($arraySQL,"orderby");
			$strSQL .= MakeSQL_Helper_OrderBy($arrayOrderBy);

			$arrayLimit = ArrayGetValue($arraySQL,"limit");
			$strSQL .= MakeSQL_Helper_Limit($arrayLimit);
			
			$strSQL .= ";";
			return $strSQL;
		}


		if ($strCommand == "INSERT")
		{
			$strSQL = "INSERT ";
			
			$strSQL .= "INTO ";
			$vInto = ArrayGetValue($arraySQL,"tables");	
			if (is_string($vInto))
			{
				$strSQL .= MakeSQL_Helper_Table($vInto);	
			}
			else
			{
				$strError = "arraySQL[into] is not a string.";
				return false;
			}
			
			$arrayValues = ArrayGetValue($arraySQL,"values");
			$strSQLValues = MakeSQL_Helper_Assignments($arrayValues,$strError);
			if ($strSQLValues == false)
			{
				return false;	
			}
			$strSQL .= " SET ".$strSQLValues; 

			$arraySQLValues = ArrayGetValue($arraySQL,"on_duplicate_key_update");
			if ($arraySQLValues === true)
			{
				$strSQL .= " ON DUPLICATE KEY UPDATE ".$strSQLValues;
			}
			else
			{
				$strSQLValues = MakeSQL_Helper_Assignments($arraySQLValues,$strError);
				if ($strSQLValues === false)
				{
					return false;	
				}
				if ($strSQLValues != "")
				{
					$strSQL .= " ON DUPLICATE KEY UPDATE ".$strSQLValues;	
				}
			}
			$strSQL .= ";";
			return $strSQL;
		}

		if ($strCommand == "REPLACE")
		{
			$strSQL = "REPLACE ";
			
			$strSQL .= "INTO ";
			$vInto = ArrayGetValue($arraySQL,"tables");	
			if (is_string($vInto))
			{
				$strSQL .= MakeSQL_Helper_Table($vInto);	
			}
			else
			{
				$strError = "arraySQL[into] is not a string.";
				return false;
			}

			$arrayValues = ArrayGetValue($arraySQL,"values");
			$strSQLValues = MakeSQL_Helper_Assignments($arrayValues,$strError);
			if ($strSQLValues == false)
			{
				return false;	
			}
			$strSQL .= " SET ".$strSQLValues; 

			$strSQL .= ";";
			return $strSQL;
		}




		if ($strCommand == "DELETE")
		{
			$strSQL = "DELETE ";
			if (ArrayGetValue($arraySQL,"calculate_found_rows") == true)
			{
				$strError = "SQL_CALC_FOUND_ROWS must not be specified";
				return false;
			}
			
			$arrayFields = ArrayGetValue($arraySQL,"fields");
			if (is_array($arrayFields))
			{
				$strError = "Fields must not be specified";
				return false;
			}

			$strSQL .= "FROM ";
			$vFrom = ArrayGetValue($arraySQL,"tables");	
			if (is_string($vFrom))
			{
				$strSQL .= MakeSQL_Helper_Table($vFrom);	
			}
			else
			{
				$strError = "arraySQL[from] is not a string.";
				return false;
			}

			$vWhere = ArrayGetValue($arraySQL,"where");
			if ($vWhere != NULL)
			{
				$strWhereCondition = MakeSQL_Helper_ConditionalExpression($vWhere,$strError);
				if ($strWhereCondition == false)
				{
					$strError = "MakeSQL_Helper_ConditionalExpression(arraySQL[where]) returned false. ".$strError;
					return false;	
				}
				$strSQL .= " WHERE ".$strWhereCondition;
			}
			
			$arrayOrderBy = ArrayGetValue($arraySQL,"orderby");
			$strSQL .= MakeSQL_Helper_OrderBy($arrayOrderBy);

			$arrayLimit = ArrayGetValue($arraySQL,"limit");
			$strSQL .= MakeSQL_Helper_Limit($arrayLimit);
			
			$strSQL .= ";";
			return $strSQL;
		}


		$strError = "Command not supported";
		return false;
	}
	
	function MakeSQL_Helper_EscapeString($strString)
	{
		// mysqli_real_escape_string() 
		
		if(empty($strString) || !is_string($strString))
		{ 
			return $strString;
		}
		$strString = str_replace(
			array('\\', "\0", "\n", "\r", "'", '"', "\x1a"),
			array('\\\\', '\\0', '\\n', '\\r', "\\'", '\\"', '\\Z'),
			$strString); 
		return StringEncodeUTF8($strString);
 } 



	function MakeSQL_Helper_Fields($vFields,&$strError = null)
	{
		$strError = "";
		$strSQL = "";
		
		if (is_array($vFields))
		{
			foreach ($vFields as $vField)
			{
				if ($strSQL != "")
				{
					$strSQL .= ", ";
				}
				if (is_array($vField))
				{
					$strTable = ArrayGetValue($vField,"table");
					$strField = ArrayGetValue($vField,"field");
					$strFieldAlias = ArrayGetValue($vField,"as");
					if ($strField == "")
					{
						$strError = "Field is not specified";
						return false;	
					}
					$strSQL .= MakeSQL_Helper_Field($strField,$strTable,$strFieldAlias);
				}
				else if (is_string($vField))
				{
					$strSQL	.= MakeSQL_Helper_Field($vField);
				}
			}
		}
		else if (is_string($vFields) && $vFields != "")
		{
			$strSQL = $vFields;
		}
		else
		{
			$strSQL = "*";
		}
		return $strSQL;
	}

	function MakeSQL_Helper_Assignment($strField,$strValue)
	{
		$strSQL = MakeSQL_Helper_Field($strField)." = ";
		if ("".$strValue == "++")
		{
			return $strSQL . "$strField + 1";
		}
		if ("".$strValue == "--")
		{
			return $strSQL . "$strField - 1";
		}
		return $strSQL . MakeSQL_Helper_Value($strValue);
	}
	
	function MakeSQL_Helper_Assignments($arraySQLValues,&$strError = null)
	{
		$strError = "";
		if (!is_array($arraySQLValues))
		{
			$arraySQLValues = array();
			$strError = "No values are specified";
			return "";
		}
		$strSQLValues = "";
		foreach ($arraySQLValues as $vValue)
		{
			if ($strSQLValues != "")
			{
				$strSQLValues .= ", ";
			}				
			if (is_array($vValue))
			{
				$strField = ArrayGetValue($vValue,"field");
				if ($strField == "")
				{
					$strError = "Field is not specified";
					return false;	
				}
				$strValue = ArrayGetValue($vValue,"value");				
				$strSQLValues .= MakeSQL_Helper_Assignment($strField,$strValue);
			}
			else
			{
				$strSQLValues .= MakeSQL_Helper_Value($vValue,$strError);
			}
		}
		return $strSQLValues;
	}

	function MakeSQL_Helper_OnDuplicateKeyUpdate($arraySQLValues,$strSQLValues,&$strError = null)
	{
		
					
		
	}


	
	function MakeSQL_Helper_Value($vValue)
	{
		// This is dangerous as it makes the system vulnerable to sql injection attacks. But this also makes sql statements impossible that contain '-quotes intentionally. A trade-off we can live with!
		/*if (strpos($strValue,"'") !== false)
		{
			return $strValue;
		}*/
		if (is_integer($vValue) || is_float($vValue))
		{
			return "".RenderNumber($vValue);
		}
		if ($vValue == "NOW()")
		{
			return $vValue;
		}
		if ($vValue == "NULL")
		{
			return $vValue;
		}
		if (IsValidDateTime($vValue))
		{
			return "'".$vValue."'";
		}
		return "'".MakeSQL_Helper_EscapeString($vValue)."'";
	}
	
	function MakeSQL_Helper_Table($strTable)
	{
		return "/*TablePrefix*/`".MakeSQL_Helper_EscapeString($strTable)."`";
	}

	function MakeSQL_Helper_Field($strField,$strTable="",$strFieldAlias="")
	{
		if ($strField == "")
		{
			return "";
		}
		return 
			(($strTable=="")?(""):(MakeSQL_Helper_Table($strTable)."."))
			."`".MakeSQL_Helper_EscapeString($strField)."`"
			.(($strFieldAlias=="")?(""):(" AS `".MakeSQL_Helper_EscapeString($strFieldAlias)."`"));
	}
	
	function MakeSQL_Helper_Limit($arrayLimit)
	{
		if (!is_array($arrayLimit))
		{
			return "";	
		}
		$nOffset = intval(ArrayGetValue($arrayLimit,"offset"));
		$nRowCount = intval(ArrayGetValue($arrayLimit,"rowcount"));
		if ($nRowCount == 0)
		{
			return "";	
		}
		$strSQL = " LIMIT ";
		if ($nOffset > 0)
		{
			$strSQL .= "$nOffset, ";
		}
		$strSQL .= "$nRowCount";
		return $strSQL;
	}

	function MakeSQL_Helper_OrderBy($arrayOrderBy)
	{
		if (!is_array($arrayOrderBy))
		{
			return "";	
		}
		$strSQL = ""; 
		if (ArrayCount($arrayOrderBy) > 0)
		{
			$strSQL .= " ORDER BY ";
			$bFirst = true;
			foreach ($arrayOrderBy as $arrayColumn)
			{
				if ($bFirst == false)
				{
					$strSQL .= ", ";
				}
				if (!is_array($arrayColumn))	
				{
					$strError = "arraySQL[orderby][] is not an array.";
					return false;	
				}
				$strField = ArrayGetValue($arrayColumn,"field");
				if ($strField == "")
				{
					$strError = "arraySQL[orderby][][field] is empty.";
					return false;	
				}
				$strTable = ArrayGetValue($arrayColumn,"table");
				$strSQL .= MakeSQL_Helper_Field($strField,$strTable);
				$strOrder = ArrayGetValue($arrayColumn,"order");
				$arrayOrders = array("","ASC","DESC");
				if (!ArrayValueExists($arrayOrders,$strOrder))
				{
					$strError = "arraySQL[orderby][][order] = $strOrder is not supported.";
					return false;
				}
				if ($strOrder != "")
				{
					$strSQL .= " ".$strOrder;
				}
				$bFirst = false;
			}
		}
		return $strSQL;
	}


	function MakeSQL_Helper_GroupBy($arrayGroupBy,&$strError = null)
	{
		$strError = "";
		$strSQL = "";
		if (is_array($arrayGroupBy) == true)
		{
			if (ArrayCount($arrayGroupBy) > 0)
			{
				$strSQL .= " GROUP BY ";
				$bFirst = true;
				foreach ($arrayGroupBy as $arrayColumn)
				{
					if ($bFirst == false)
					{
						$strSQL .= ", ";
					}
					if (!is_array($arrayColumn))	
					{
						$strError = "arraySQL[groupby][] is not an array.";
						return false;	
					}
					$strField = ArrayGetValue($arrayColumn,"field");
					if ($strField == "")
					{
						$strError = "arraySQL[groupby][][field] is empty.";
						return false;	
					}
					$strTable = ArrayGetValue($arrayColumn,"table");
					$strSQL .= MakeSQL_Helper_Field($strField,$strTable);
					$strOrder = ArrayGetValue($arrayColumn,"order");
					$arrayOrders = array("","ASC","DESC");
					if (!ArrayValueExists($arrayOrders,$strOrder))
					{
						$strError = "arraySQL[groupby][][order] = $strOrder is not supported.";
						return false;
					}
					if ($strOrder != "")
					{
						$strSQL .= " ".$strOrder;
					}
					$bFirst = false;
				}
			}
		}
		return $strSQL;
	}

	function MakeSQL_Insert(
		$strTable,
		$arrayFields,
		$bOnDuplicateKeyUpdate = false)
	{
		if (!is_array($arrayFields))
		{
			$arrayFields = array();
		}
		$arraySQLValues = array();
		foreach ($arrayFields as $strField => $strValue)
		{
			$arraySQLValues[] = array(
				"field" => $strField,
				"value" => $strValue);
		}
		$arraySQL = array(
			"command" => "INSERT",
			"tables" => $strTable,
			"values" => $arraySQLValues,
			"on_duplicate_key_update" => $bOnDuplicateKeyUpdate);
		return MakeSQL($arraySQL);		
	}

	
	function MakeSQL_Replace(
		$strTable,
		$arrayFields)
	{
		if (!is_array($arrayFields))	
		{
			$arrayFields = array();
		}
		$strSQL = "REPLACE INTO ".MakeSQL_Helper_Table($strTable)." ";
		
		$nCount = ArrayCount($arrayFields);
		
		if ($nCount > 0)
		{
			$strSQL .= "SET ";
		}

		$nIndex = 0;
		foreach ($arrayFields as $strField => $strValue)
		{
			$strSQL .= MakeSQL_Helper_Assignment($strField,$strValue);
			if ($nIndex < $nCount-1)
			{
				$strSQL .= ", ";
			}
			$nIndex++;
		}
		
		$strSQL .= ";";
		
		return $strSQL;
	}
	
	function MakeSQL_Delete(
		$strTable,
		$vConditions)
	{
		$arraySQL = array(
			"command" => "DELETE",
			"tables" => $strTable,
			"where" => $vConditions
			);
		return MakeSQL($arraySQL);
	}
	
	function MakeSQL_Update(
		$strTable,
		$arrayFields,
		$strCondition)
	{
		$strSQL = "UPDATE ".MakeSQL_Helper_Table($strTable)." SET ";
		
		$nCount = ArrayCount($arrayFields);

		$nIndex = 0;
		foreach ($arrayFields as $strField => $strValue)
		{
			$strSQL .= MakeSQL_Helper_Assignment($strField,$strValue);
			if ($nIndex < $nCount-1)
			{
				$strSQL .= ", ";
			}
			$nIndex++;
		}
		$strSQL .= " WHERE $strCondition";
		return $strSQL;	
	}
	
	function MakeSQL_Select(
		$strTable,
		$vFields,
		$vConditions)
	{
		$arraySQL = array(
			"command" => "SELECT",
			"tables" => $strTable,
			"fields" => $vFields,
			"where" => $vConditions
			);
		return MakeSQL($arraySQL);		
		
	}
	
	function MakeSQL_PrefixTables($strSQL,$strTablePrefix = "")
	{
		if ($strTablePrefix == "")
		{
			return $strSQL;
		}
		return ReplaceString($strSQL,"/*TablePrefix*/`","`".$strTablePrefix);
	}
	
	function SQL_GetReservedNames()
	{
		return array(
			"ACCESSIBLE","ADD","ALL","ALTER","ANALYZE","AND","AS","ASC","ASENSITIVE","BEFORE","BETWEEN","BIGINT","BINARY","BLOB","BOTH","BY","CALL","CASCADE","CASE","CHANGE","CHAR","CHARACTER","CHECK","COLLATE","COLUMN","CONDITION","CONNECTION","CONSTRAINT","CONTINUE","CONVERT","CREATE","CROSS","CURRENT_DATE","CURRENT_TIME","CURRENT_TIMESTAMP","CURRENT_USER","CURSOR","DATABASE","DATABASES","DAY_HOUR","DAY_MICROSECOND","DAY_MINUTE","DAY_SECOND","DEC","DECIMAL","DECLARE","DEFAULT","DELAYED","DELETE","DESC","DESCRIBE","DETERMINISTIC","DISTINCT","DISTINCTROW","DIV","DOUBLE","DROP","DUAL","EACH","ELSE","ELSEIF","ENCLOSED","ESCAPED","EXISTS","EXIT","EXPLAIN","FALSE","FETCH","FLOAT","FLOAT4","FLOAT8","FOR","FORCE","FOREIGN","FROM","FULLTEXT","GOTO","GRANT","GROUP","HAVING","HIGH_PRIORITY","HOUR_MICROSECOND","HOUR_MINUTE","HOUR_SECOND","IF","IGNORE","IN","INDEX","INFILE","INNER","INOUT","INSENSITIVE","INSERT","INT","INT1","INT2","INT3","INT4","INT8","INTEGER","INTERVAL","INTO","IS","ITERATE","JOIN","KEY","KEYS","KILL","LABEL","LEADING","LEAVE","LEFT","LIKE","LIMIT","LINEAR","LINES","LOAD","LOCALTIME","LOCALTIMESTAMP","LOCK","LONG","LONGBLOB","LONGTEXT","LOOP","LOW_PRIORITY","MASTER_SSL_VERIFY_SERVER_CERT","MATCH","MEDIUMBLOB","MEDIUMINT","MEDIUMTEXT","MIDDLEINT","MINUTE_MICROSECOND","MINUTE_SECOND","MOD","MODIFIES","NATURAL","NOT","NO_WRITE_TO_BINLOG","NULL","NUMERIC","ON","OPTIMIZE","OPTION","OPTIONALLY","OR","ORDER","OUT","OUTER","OUTFILE","PRECISION","PRIMARY","PROCEDURE","PURGE","RANGE","READ","READS","READ_ONLY","READ_WRITE","REAL","REFERENCES","REGEXP","RELEASE","RENAME","REPEAT","REPLACE","REQUIRE","RESTRICT","RETURN","REVOKE","RIGHT","RLIKE","SCHEMA","SCHEMAS","SECOND_MICROSECOND","SELECT","SENSITIVE","SEPARATOR","SET","SHOW","SMALLINT","SPATIAL","SPECIFIC","SQL","SQLEXCEPTION","SQLSTATE","SQLWARNING","SQL_BIG_RESULT","SQL_CALC_FOUND_ROWS","SQL_SMALL_RESULT","SSL","STARTING","STRAIGHT_JOIN","TABLE","TERMINATED","THEN","TINYBLOB","TINYINT","TINYTEXT","TO","TRAILING","TRIGGER","TRUE","UNDO","UNION","UNIQUE","UNLOCK","UNSIGNED","UPDATE","UPGRADE","USAGE","USE","USING","UTC_DATE","UTC_TIME","UTC_TIMESTAMP","VALUES","VARBINARY","VARCHAR","VARCHARACTER","VARYING","WHEN","WHERE","WHILE","WITH","WRITE","XOR","YEAR_MONTH","ZEROFILL");	
	}
	
	function SQL_IsValidTableName($strTable)
	{
		if ($strTable == "")
		{
			return false;	
		}
		if (StringLength($strTable) > 64)
		{
			return false;	
		}
		if (StringCheckCharSet($strTable,CHARSET_DBTABLE) == false)
		{
			return false;				
		}
		if (StringCheckCharSet(StringSection($strTable,0,1),CHARSET_ALPHA) == false)
		{
			return false;				
		}		
		if (ArrayValueExists(SQL_GetReservedNames(),StringUpperCase($strTable)) == true)
		{
			return false;
		}
		return true;
	}

	
	
