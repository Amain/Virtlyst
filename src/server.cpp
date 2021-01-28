/*
 * Copyright (C) 2018 Daniel Nicoletti <dantti12@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "server.h"

#include "virtlyst.h"

#include "lib/generic.h"

#include <Cutelyst/Plugins/StatusMessage>
#include <Cutelyst/Plugins/Utils/Sql>

#include <QSqlQuery>
#include <QSqlError>

#include <QUuid>
#include <QLoggingCategory>

Server::Server(Virtlyst *parent) : Controller(parent)
  , m_virtlyst(parent)
{

}

void Server::index(Context *c)
{
    c->setStash(QStringLiteral("template"), QStringLiteral("servers.html"));

    if (c->request()->isPost()) {
        const ParamsMultiMap params = c->request()->bodyParameters();
        if (params.contains(QStringLiteral("host_tcp_add"))) {
            const QString hostname = params[QStringLiteral("hostname")];
            const QString name = params[QStringLiteral("name")];
            const QString login = params[QStringLiteral("login")];
            const QString password = params[QStringLiteral("password")];
            const int driver = params[QStringLiteral("driver")].toInt();

            createServer(ServerConn::ConnTCP, driver, name, hostname, login, password);
        } else if (params.contains(QStringLiteral("host_ssh_add"))) {
            const QString hostname = params[QStringLiteral("hostname")];
            const QString name = params[QStringLiteral("name")];
            const QString login = params[QStringLiteral("login")];
            const int driver = params[QStringLiteral("driver")].toInt();

            createServer(ServerConn::ConnSSH, driver, name, hostname, login, QString());
        } else if (params.contains(QStringLiteral("host_tls_add"))) {
            const QString hostname = params[QStringLiteral("hostname")];
            const QString name = params[QStringLiteral("name")];
            const QString login = params[QStringLiteral("login")];
            const QString password = params[QStringLiteral("password")];
            const int driver = params[QStringLiteral("driver")].toInt();

            createServer(ServerConn::ConnTLS, driver, name, hostname, login, password);
        } else if (params.contains(QStringLiteral("host_socket_add"))) {
            const QString name = params[QStringLiteral("name")];
            const int driver = params[QStringLiteral("driver")].toInt();

            createServer(ServerConn::ConnSocket, driver, name, QStringLiteral("localhost"), QLatin1String(""), QString());
        } else if (params.contains(QStringLiteral("host_edit"))) {
            const int hostId = params[QStringLiteral("host_id")].toInt();
            const QString hostname = params[QStringLiteral("hostname")];
            const QString name = params[QStringLiteral("name")];
            const QString login = params[QStringLiteral("login")];
            const QString password = params[QStringLiteral("password")];

            updateServer(hostId, name, hostname, login, password);
        } else if (params.contains(QStringLiteral("host_del"))) {
            const int hostId = params[QStringLiteral("host_id")].toInt();

            deleteServer(hostId);
        }
        c->response()->redirect(c->uriFor(CActionFor(QStringLiteral("index"))));
        return;
    }

    ParamsMultiMap drivers = QtEnumToParamsMultiMap<ServerConn::DriverType>();

    c->setStash(QStringLiteral("servers"), QVariant::fromValue(m_virtlyst->servers(c)));
    c->setStash(QStringLiteral("drivers"), drivers);
}

void Server::createServer(int type, int driver, const QString &name, const QString &hostname, const QString &login, const QString &password)
{    
    /* Input validation: driver */
    ParamsMultiMap drivers = QtEnumToParamsMultiMap<ServerConn::DriverType>();
    if(! drivers.contains(QString::number(driver))) {
        qWarning() << "Incorrect hypervisor driver supplied";
        return;
    }

    QSqlQuery query = CPreparedSqlQueryThreadForDB(
                QStringLiteral("INSERT INTO servers_compute "
                               "(type, name, driver, hostname, login, password) "
                               "VALUES "
                               "(:type, :name, :driver, :hostname, :login, :password)"),
                QStringLiteral("virtlyst"));
    query.bindValue(QStringLiteral(":driver"), driver );
    query.bindValue(QStringLiteral(":type"), type);
    query.bindValue(QStringLiteral(":name"), name);
    query.bindValue(QStringLiteral(":hostname"), hostname);
    query.bindValue(QStringLiteral(":login"), login);
    query.bindValue(QStringLiteral(":password"), password);
    if (!query.exec()) {
        qWarning() << "Failed to add connection" << query.lastError().databaseText();
    }
}

void Server::updateServer(int id, const QString &name, const QString &hostname, const QString &login, const QString &password)
{
    QSqlQuery query = CPreparedSqlQueryThreadForDB(
                QStringLiteral("UPDATE servers_compute "
                               "SET "
                               "name = :name, "
                               "hostname = :hostname, "
                               "login = :login, "
                               "password = :password "
                               "WHERE id = :id"),
                QStringLiteral("virtlyst"));
    query.bindValue(QStringLiteral(":id"), id);
    query.bindValue(QStringLiteral(":name"), name);
    query.bindValue(QStringLiteral(":hostname"), hostname);
    query.bindValue(QStringLiteral(":login"), login);
    query.bindValue(QStringLiteral(":password"), password);
    if (!query.exec()) {
        qWarning() << "Failed to update connection" << query.lastError().databaseText();
    }
}

void Server::deleteServer(int id)
{
    QSqlQuery query = CPreparedSqlQueryThreadForDB(
                QStringLiteral("DELETE FROM servers_compute WHERE id = :id"),
                QStringLiteral("virtlyst"));
    query.bindValue(QStringLiteral(":id"), id);
    if (!query.exec()) {
        qWarning() << "Failed to delete connection" << query.lastError().databaseText();
    }
}
