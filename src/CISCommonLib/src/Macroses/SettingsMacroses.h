#ifndef SETTINGSMACROSES_H
#define SETTINGSMACROSES_H


#define INT_SETTING(name, paramName, defaultValue)\
	virtual void set##name(const qint32 value)\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	settings.setValue(#paramName, value);\
	}\
	\
	virtual qint32 get##name()\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	qint32 value = defaultValue;\
	if (!settings.contains(#paramName)) {\
		QSettings global_settings(m_systemSettingsFile, QSettings::IniFormat);\
		value = global_settings.value(#paramName, defaultValue).toInt();\
	} else {\
		value = settings.value(#paramName, defaultValue).toInt();\
	}\
	return value;\
	}\


#define UINT_SETTING(name, paramName, defaultValue)\
	virtual void set##name(const quint32 value)\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	settings.setValue(#paramName, value);\
	}\
	\
	virtual quint32 get##name()\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	quint32 value = defaultValue;\
	if (!settings.contains(#paramName)) {\
		QSettings global_settings(m_systemSettingsFile, QSettings::IniFormat);\
		value = global_settings.value(#paramName, defaultValue).toUInt();\
	} else {\
		value = settings.value(#paramName, defaultValue).toUInt();\
	}\
	return value;\
	}\

#define UINT_SETTING_ID(name, sectionName, keyName, defaultValue)\
	virtual void set##name(const quint32 id, const quint32 value)\
	{\
	setValueForSectionIdKey(#sectionName, #keyName, id, value);\
	}\
	\
	virtual quint32 get##name(const quint32 id)\
	{\
	QVariant value = getSectionKeyById(#sectionName, #keyName, id, #defaultValue);\
	return value.toUInt();\
	}

#define STRING_SETTING(name, paramName, defaultValue)\
	virtual void set##name(const QString& value)\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	settings.setValue(#paramName, value);\
	}\
	\
	virtual QString get##name()\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	QString value = defaultValue;\
	if (!settings.contains(#paramName)) {\
		QSettings global_settings(m_systemSettingsFile, QSettings::IniFormat);\
		value = global_settings.value(#paramName, defaultValue).toString();\
	} else {\
		value = settings.value(#paramName, defaultValue).toString();\
	}\
	return value;\
	}\

#define STRING_SETTING_ID(name, sectionName, keyName, defaultValue)\
	virtual void set##name(const quint32 id, const QString& value)\
	{\
	setValueForSectionIdKey(#sectionName, #keyName, id, value);\
	}\
	\
	virtual QString get##name(const quint32 id)\
	{\
	QVariant value = getSectionKeyById(#sectionName, #keyName, id, #defaultValue);\
	return value.toString();\
	}\

#define STRING_SETTING_UTF8(name, paramName, defaultValue)\
	virtual void set##name(const QString& value)\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	settings.setIniCodec("UTF-8");\
	settings.setValue(#paramName, value);\
	}\
	\
	virtual QString get##name()\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	QString value = defaultValue;\
	if (!settings.contains(#paramName)) {\
		QSettings global_settings(m_systemSettingsFile, QSettings::IniFormat);\
		global_settings.setIniCodec("UTF-8");\
		value = global_settings.value(#paramName, defaultValue).toString();\
	} else {\
		settings.setIniCodec("UTF-8");\
		value = settings.value(#paramName, defaultValue).toString();\
	}\
	return value;\
	}\

#define QVARIANT_SETTING(name, paramName, defaultValue)\
	virtual void set##name(const QVariant& value)\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	settings.setValue(#paramName, value);\
	}\
	\
	virtual QVariant get##name()\
	{\
	QMutexLocker mutexLocker(&m_mutex);\
	QSettings settings(m_userSettingsFile, QSettings::IniFormat);\
	QVariant value = defaultValue;\
	if (!settings.contains(#paramName)) {\
		QSettings global_settings(m_systemSettingsFile, QSettings::IniFormat);\
		value = global_settings.value(#paramName, defaultValue);\
	} else {\
		value = settings.value(#paramName, defaultValue);\
	}\
	return value;\
	}\

#endif // SETTINGSMACROSES_H
