#include "testawssignaturev2.h"

#include "../../src/core/awsbasiccredentials.h"
#include "../../src/core/awssignaturev2.h"
#include "../../src/core/awssignaturev2_p.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

Q_DECLARE_METATYPE(QNetworkAccessManager::Operation)

void TestAwsSignatureV2::sign_data() {
    QTest::addColumn<QNetworkAccessManager::Operation>("operation");
    QTest::addColumn<QNetworkRequest>("request");
    QTest::addColumn<QString>("accessKeyId");
    QTest::addColumn<QString>("secretKey");
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<QNetworkRequest>("expected");

    //QTest::newRow("null") << QNetworkAccessManager::GetOperation << QNetworkRequest()
    //                      << QString("") << QString("") << QByteArray() << QNetworkRequest();

    { // Official example from http://docs.aws.amazon.com/general/latest/gr/signature-version-2.html
        QTest::newRow("elasticmapreduce")
            << QNetworkAccessManager::GetOperation
            << QNetworkRequest(QUrl("https://elasticmapreduce.amazonaws.com?Action=DescribeJobFlows"
                                    "&Version=2009-03-31"
                                    "&AWSAccessKeyId=AKIAIOSFODNN7EXAMPLE"
                                    "&SignatureVersion=2"
                                    "&SignatureMethod=HmacSHA256"
                                    "&Timestamp=2011-10-03T15%3A19%3A30"))
            << "AKIAIOSFODNN7EXAMPLE"
            << "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY"
            << QByteArray()
            << QNetworkRequest(QUrl("https://elasticmapreduce.amazonaws.com?Action=DescribeJobFlows"
                                    "&Version=2009-03-31"
                                    "&AWSAccessKeyId=AKIAIOSFODNN7EXAMPLE"
                                    "&SignatureVersion=2"
                                    "&SignatureMethod=HmacSHA256"
                                    "&Timestamp=2011-10-03T15%3A19%3A30"
                                    "&Signature=i91nKc4PWAt0JJIdXwz9HxZCJDdiy6cf%2FMj6vPxyYIs%3D"));
    }

}

void TestAwsSignatureV2::sign() {
    QFETCH(QNetworkAccessManager::Operation, operation);
    QFETCH(QNetworkRequest, request);
    QFETCH(QString, accessKeyId);
    QFETCH(QString, secretKey);
    QFETCH(QByteArray, data);
    QFETCH(QNetworkRequest, expected);

    const AwsBasicCredentials credentials(accessKeyId, secretKey);

    AwsSignatureV2 signature;
    signature.sign(operation, request, credentials, data);
    //QCOMPARE(request.url(), expected.url());
    //QCOMPARE(request, expected);

    QBENCHMARK {
        QFETCH(QNetworkRequest, request);
        signature.sign(operation, request, credentials, data);
    }
}

void TestAwsSignatureV2::canonicalPath_data() {
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<QString>("path");

    QTest::newRow("invalid"     ) << QUrl() << QString("/");
    QTest::newRow("none"        ) << QUrl("http://example.com")             << QString("/");
    QTest::newRow("/"           ) << QUrl("http://example.com/")            << QString("/");
    QTest::newRow("/123"        ) << QUrl("http://example.com/123")         << QString("/123");
    QTest::newRow("/123/"       ) << QUrl("http://example.com/123/")        << QString("/123/");
    QTest::newRow("/foo/bar/baz") << QUrl("http://example.com/foo/bar/baz") << QString("/foo/bar/baz");
}

void TestAwsSignatureV2::canonicalPath() {
    QFETCH(QUrl, url);
    QFETCH(QString, path);
    AwsSignatureV2Private signature(NULL);
    QCOMPARE(signature.canonicalPath(url), path);
}

void TestAwsSignatureV2::canonicalRequest_data() {
    QTest::addColumn<QNetworkAccessManager::Operation>("operation");
    QTest::addColumn<QNetworkRequest>("request");
    QTest::addColumn<QString>("expected");

    // Official example from http://docs.aws.amazon.com/general/latest/gr/signature-version-2.html
    QTest::newRow("lasticmapreduce")
        << QNetworkAccessManager::GetOperation
        << QNetworkRequest(QUrl("https://elasticmapreduce.amazonaws.com?Action=DescribeJobFlows"
                                "&Version=2009-03-31"
                                "&AWSAccessKeyId=AKIAIOSFODNN7EXAMPLE"
                                "&SignatureVersion=2"
                                "&SignatureMethod=HmacSHA256"
                                "&Timestamp=2011-10-03T15%3A19%3A30"))
        << QString("GET\n"
                   "elasticmapreduce.amazonaws.com\n"
                   "/\n"
                   "AWSAccessKeyId=AKIAIOSFODNN7EXAMPLE&Action=DescribeJobFlows&SignatureMethod=HmacSHA256&SignatureVersion=2&Timestamp=2011-10-03T15%3A19%3A30&Version=2009-03-31");
}

void TestAwsSignatureV2::canonicalRequest() {
    QFETCH(QNetworkAccessManager::Operation, operation);
    QFETCH(QNetworkRequest, request);
    QFETCH(QString, expected);

    AwsSignatureV2Private signature(NULL);
    QCOMPARE(signature.canonicalRequest(operation, request.url()), expected);
}