/***************************************************************************
  qgsprocessingparametermeshdataset.cpp
  ---------------------
  Date                 : October 2020
  Copyright            : (C) 2020 by Vincent Cloarec
  Email                : vcloarec at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsprocessingparametermeshdataset.h"

/// @cond PRIVATE
///
QgsProcessingParameterMeshDatasetGroups::QgsProcessingParameterMeshDatasetGroups( const QString &name,
    const QString &description,
    const QString &meshLayerParameterName,
    const QgsMeshDatasetGroupMetadata::DataType dataType,
    bool optional ):
  QgsProcessingParameterDefinition( name, description, QVariant(), optional, QString() ),
  mMeshLayerParameterName( meshLayerParameterName ),
  mDataType( dataType )
{
}

QgsProcessingParameterDefinition *QgsProcessingParameterMeshDatasetGroups::clone() const
{
  return new QgsProcessingParameterMeshDatasetGroups( name(), description(), mMeshLayerParameterName, mDataType );
}

QString QgsProcessingParameterMeshDatasetGroups::type() const
{
  return typeName();
}

bool QgsProcessingParameterMeshDatasetGroups::checkValueIsAcceptable( const QVariant &input, QgsProcessingContext *context ) const
{
  Q_UNUSED( context );
  return valueIsAcceptable( input, mFlags & FlagOptional );
}

QString QgsProcessingParameterMeshDatasetGroups::valueAsPythonString( const QVariant &value, QgsProcessingContext &context ) const
{
  Q_UNUSED( context );
  QStringList parts;
  const QVariantList variantDatasetGroupIndexes = value.toList();
  for ( const QVariant &variantIndex : variantDatasetGroupIndexes )
    parts.append( QString::number( variantIndex.toInt() ) );

  return parts.join( ',' ).prepend( '[' ).append( ']' );
}

QString QgsProcessingParameterMeshDatasetGroups::asPythonString( QgsProcessing::PythonOutputType outputType ) const
{
  switch ( outputType )
  {
    case QgsProcessing::PythonQgsProcessingAlgorithmSubclass:
    {
      QString code = QStringLiteral( "QgsProcessingParameterMeshDatasetGroups('%1', '%2'" )
                     .arg( name(), description() );
      if ( !mMeshLayerParameterName.isEmpty() )
        code += QStringLiteral( ", meshLayerParameterName=%1" ).arg( QgsProcessingUtils::stringToPythonLiteral( mMeshLayerParameterName ) );

      switch ( mDataType )
      {
        case QgsMeshDatasetGroupMetadata::DataOnFaces:
          code += QStringLiteral( ", dataType=QgsMeshDatasetGroupMetadata.DataOnFaces" );
          break;
        case QgsMeshDatasetGroupMetadata::DataOnVertices:
          code += QStringLiteral( ", dataType=QgsMeshDatasetGroupMetadata.DataOnVertices" );
          break;
        case QgsMeshDatasetGroupMetadata::DataOnVolumes:
          code += QStringLiteral( ", dataType=QgsMeshDatasetGroupMetadata.DataOnVolumes" );
          break;
        case QgsMeshDatasetGroupMetadata::DataOnEdges:
          code += QStringLiteral( ", dataType=QgsMeshDatasetGroupMetadata.DataOnEdges" );
          break;
      }

      if ( mFlags & FlagOptional )
        code += QStringLiteral( ", optional=True" );
      code += ')';
      return code;
    }
  }
  return QString();
}

QStringList QgsProcessingParameterMeshDatasetGroups::dependsOnOtherParameters() const
{
  if ( mMeshLayerParameterName.isEmpty() )
    return QStringList();
  else
    return QStringList() << mMeshLayerParameterName;
}

QString QgsProcessingParameterMeshDatasetGroups::meshLayerParameterName() const
{
  return mMeshLayerParameterName;
}

QList<int> QgsProcessingParameterMeshDatasetGroups::valueAsDatasetGroup( const QVariant &value )
{
  if ( !valueIsAcceptable( value, true ) )
    return QList<int>();
  QVariantList list = value.toList();
  QList<int> ret;
  for ( const QVariant &v : list )
    ret.append( v.toInt() );

  return ret;
}

bool QgsProcessingParameterMeshDatasetGroups::valueIsAcceptable( const QVariant &input, bool allowEmpty )
{
  if ( !input.isValid() )
    return allowEmpty;

  if ( input.type() != QVariant::List )
    return false;
  const QVariantList list = input.toList();

  if ( !allowEmpty && list.isEmpty() )
    return false;

  for ( const QVariant &var : list )
    if ( var.type() != QVariant::Int )
      return false;

  return true;
}

QgsProcessingParameterMeshDatasetTime::QgsProcessingParameterMeshDatasetTime( const QString &name,
    const QString &description,
    const QString &meshLayerParameterName,
    const QString &datasetGroupParameterName )
  : QgsProcessingParameterDefinition( name, description, QVariant() )
  , mMeshLayerParameterName( meshLayerParameterName )
  , mDatasetGroupParameterName( datasetGroupParameterName )
{

}

QgsProcessingParameterDefinition *QgsProcessingParameterMeshDatasetTime::clone() const
{
  return new QgsProcessingParameterMeshDatasetTime( name(), description(), mMeshLayerParameterName, mDatasetGroupParameterName );
}

QString QgsProcessingParameterMeshDatasetTime::type() const
{
  return typeName();
}

bool QgsProcessingParameterMeshDatasetTime::checkValueIsAcceptable( const QVariant &input, QgsProcessingContext *context ) const
{
  Q_UNUSED( context );
  return valueIsAcceptable( input, mFlags & FlagOptional );
}

QString QgsProcessingParameterMeshDatasetTime::valueAsPythonString( const QVariant &value, QgsProcessingContext &context ) const
{
  Q_UNUSED( context );
  QStringList parts;
  const QVariantMap variantTimeDataset = value.toMap();
  parts << QStringLiteral( "'type': " ) +  QgsProcessingUtils::variantToPythonLiteral( variantTimeDataset.value( QStringLiteral( "type" ) ).toString() );

  if ( variantTimeDataset.value( QStringLiteral( "type" ) ) == QStringLiteral( "dataset-time-step" ) )
  {
    QVariantList datasetIndex = variantTimeDataset.value( QStringLiteral( "value" ) ).toList();
    parts << QStringLiteral( "'value': " ) + QString( "QgsMeshDatasetIndex(%1,%2)" ).arg( datasetIndex.at( 0 ).toString() ).arg( datasetIndex.at( 1 ).toString() );
  }
  else if ( variantTimeDataset.value( QStringLiteral( "type" ) ) == QStringLiteral( "defined-date-time" ) )
  {
    parts << QStringLiteral( "'value': " ) + QgsProcessingUtils::variantToPythonLiteral( variantTimeDataset.value( QStringLiteral( "value" ) ) );
  }

  return parts.join( ',' ).prepend( '{' ).append( '}' );
}

QString QgsProcessingParameterMeshDatasetTime::asPythonString( QgsProcessing::PythonOutputType outputType ) const
{
  switch ( outputType )
  {
    case QgsProcessing::PythonQgsProcessingAlgorithmSubclass:
    {
      QString code = QStringLiteral( "QgsProcessingParameterMeshDatasetTime('%1', '%2'" )
                     .arg( name(), description() );
      if ( !mMeshLayerParameterName.isEmpty() )
        code += QStringLiteral( ", meshLayerParameterName=%1" ).arg( QgsProcessingUtils::stringToPythonLiteral( mMeshLayerParameterName ) );

      if ( !mDatasetGroupParameterName.isEmpty() )
        code += QStringLiteral( ", datasetGroupParameterName=%1" ).arg( QgsProcessingUtils::stringToPythonLiteral( mDatasetGroupParameterName ) );

      if ( mFlags & FlagOptional )
        code += QStringLiteral( ", optional=True" );
      code += ')';
      return code;
    }
  }
  return QString();
}

QStringList QgsProcessingParameterMeshDatasetTime::dependsOnOtherParameters() const
{
  QStringList otherParameters;
  if ( !mMeshLayerParameterName.isEmpty() )
    otherParameters << mMeshLayerParameterName;

  if ( !mDatasetGroupParameterName.isEmpty() )
    otherParameters << mMeshLayerParameterName << mDatasetGroupParameterName;

  return otherParameters;
}

QString QgsProcessingParameterMeshDatasetTime::meshLayerParameterName() const
{
  return mMeshLayerParameterName;
}

QString QgsProcessingParameterMeshDatasetTime::datasetGroupParameterName() const
{
  return mDatasetGroupParameterName;
}

QString QgsProcessingParameterMeshDatasetTime::valueAsTimeType( const QVariant &value )
{
  if ( !valueIsAcceptable( value, false ) )
    return QString();

  return value.toMap().value( QStringLiteral( "type" ) ).toString();
}

QgsMeshDatasetIndex QgsProcessingParameterMeshDatasetTime::timeValueAsDatasetIndex( const QVariant &value )
{
  if ( !valueIsAcceptable( value, false ) || valueAsTimeType( value ) != QStringLiteral( "dataset-time-step" ) )
    return QgsMeshDatasetIndex( -1, -1 );

  QVariantList list = value.toMap().value( QStringLiteral( "value" ) ).toList();
  return QgsMeshDatasetIndex( list.at( 0 ).toInt(), list.at( 1 ).toInt() );
}

QDateTime QgsProcessingParameterMeshDatasetTime::timeValueAsDefinedDateTime( const QVariant &value )
{
  if ( !valueIsAcceptable( value, false ) && valueAsTimeType( value ) != QStringLiteral( "defined-date-time" ) )
    return QDateTime();

  return value.toMap().value( QStringLiteral( "value" ) ).toDateTime();
}

bool QgsProcessingParameterMeshDatasetTime::valueIsAcceptable( const QVariant &input, bool allowEmpty )
{
  if ( !input.isValid() )
    return allowEmpty;

  if ( input.type() != QVariant::Map )
    return false;
  const QVariantMap map = input.toMap();
  if ( ! map.contains( QStringLiteral( "type" ) ) )
    return false;

  QString type = map.value( QStringLiteral( "type" ) ).toString();
  QVariant value = map.value( QStringLiteral( "value" ) );

  if ( type == QStringLiteral( "static" ) || type == QStringLiteral( "current-context-time" ) )
    return true;

  if ( type == QStringLiteral( "dataset-time-step" ) )
  {
    if ( value.type() != QVariant::List )
      return false;
    QVariantList list = value.toList();
    if ( value.toList().count() != 2 )
      return false;
    if ( list.at( 0 ).type() != QVariant::Int || list.at( 1 ).type() != QVariant::Int )
      return false;
  }
  else if ( type == QStringLiteral( "defined-date-time" ) )
  {
    if ( value.type() != QVariant::DateTime )
      return false;
  }
  else
    return false;

  return true;
}

/// @endcond PRIVATE
