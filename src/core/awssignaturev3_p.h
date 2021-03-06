/*
    Copyright 2013-2015 Paul Colby

    This file is part of libqtaws.

    Libqtaws is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Libqtaws is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libqtaws.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AWSSIGNATUREV3_P_H
#define AWSSIGNATUREV3_P_H

#include "qtawsglobal.h"
#include "awsabstractsignature_p.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>

QTAWS_BEGIN_NAMESPACE

class AwsSignatureV3;

class QTAWS_EXPORT AwsSignatureV3Private : public AwsAbstractSignaturePrivate {

public:
    AwsSignatureV3Private(const QCryptographicHash::Algorithm hashAlgorithm, AwsSignatureV3 * const q);

    void setAuthorizationHeader(const AwsAbstractCredentials &credentials,
                                const QNetworkAccessManager::Operation operation,
                                QNetworkRequest &request, const QByteArray &payload) const;

    void setDateHeader(QNetworkRequest &request, const QDateTime &dateTime = QDateTime::currentDateTimeUtc()) const;

protected:
    const QCryptographicHash::Algorithm hashAlgorithm; ///< Hash algorithm to use when signing.

    QByteArray algorithmDesignation(const QCryptographicHash::Algorithm algorithm) const;

    QByteArray authorizationHeaderValue(const AwsAbstractCredentials &credentials,
                                        const QNetworkAccessManager::Operation operation,
                                        QNetworkRequest &request, const QByteArray &payload) const;

    QByteArray canonicalHeader(const QByteArray &headerName, const QByteArray &headerValue) const;

    QByteArray canonicalHeaders(const QNetworkRequest &request, QByteArray * const signedHeaders) const;

    QByteArray canonicalRequest(const QNetworkAccessManager::Operation operation, const QNetworkRequest &request,
                                const QByteArray &payload, QByteArray * const signedHeaders) const;

    static inline bool isHttps(const QNetworkRequest &request);

private:
    Q_DECLARE_PUBLIC(AwsSignatureV3)
    friend class TestAwsSignatureV3;
};

QTAWS_END_NAMESPACE

#endif
