;--------Components Section-----------------
; Optionally installing Server
;-------------------------------------------
Section /o "Mysql Installer" MySqlInst
    ExecWait '$INSTDIR\sql\mysql.msi'
	ExecWait '$INSTDIR\sql\create_db.cmd root root'
	ExecWait '$INSTDIR\sql2\create_db.cmd root root'
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_InstService ${LANG_ENGLISH} "Install MySql server"
  LangString DESC_InstService ${LANG_RUSSIAN} "Установить MySql сервер, пользователь, пароль - root/root"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${MySqlInst} $(DESC_InstService)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
