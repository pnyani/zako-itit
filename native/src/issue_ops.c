#include "issue_ops.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_issue(const char *name, const char *detail, IssueTag tag, const char *user_id) {
    sqlite3 *db = get_database();
    if (db == NULL) {
        return -1;
    }

    const char *sql = "INSERT INTO zako (tag, status, name, detail, discord) VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, tag);
    sqlite3_bind_int(stmt, 2, STATUS_PROPOSED);
    sqlite3_bind_text(stmt, 3, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, detail, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, user_id, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    return (int)sqlite3_last_insert_rowid(db);
}

int get_issue_by_id(int id, Issue *issue) {
    sqlite3 *db = get_database();
    if (db == NULL || issue == NULL) {
        return 0;
    }

    const char *sql = "SELECT id, tag, status, name, detail, discord FROM zako WHERE id = ?";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        issue->id = sqlite3_column_int(stmt, 0);
        issue->tag = (IssueTag)sqlite3_column_int(stmt, 1);
        issue->status = (IssueStatus)sqlite3_column_int(stmt, 2);

        const char *name = (const char *)sqlite3_column_text(stmt, 3);
        const char *detail = (const char *)sqlite3_column_text(stmt, 4);
        const char *user_id = (const char *)sqlite3_column_text(stmt, 5);

        strncpy(issue->name, name ? name : "", sizeof(issue->name) - 1);
        strncpy(issue->detail, detail ? detail : "", sizeof(issue->detail) - 1);
        strncpy(issue->user_id, user_id ? user_id : "", sizeof(issue->user_id) - 1);

        issue->name[sizeof(issue->name) - 1] = '\0';
        issue->detail[sizeof(issue->detail) - 1] = '\0';
        issue->user_id[sizeof(issue->user_id) - 1] = '\0';

        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int list_issues(int tag_filter, int status_filter, int use_tag_filter, int use_status_filter, Issue **issues, int *count) {
    sqlite3 *db = get_database();
    if (db == NULL || issues == NULL || count == NULL) {
        return 0;
    }

    char sql[512];
    int len = snprintf(sql, sizeof(sql), "SELECT id, tag, status, name, detail, discord FROM zako WHERE 1=1");

    if (use_tag_filter && len > 0 && (size_t)len < sizeof(sql)) {
        len += snprintf(sql + len, sizeof(sql) - len, " AND tag = ?");
    }
    if (use_status_filter && len > 0 && (size_t)len < sizeof(sql)) {
        len += snprintf(sql + len, sizeof(sql) - len, " AND status = ?");
    }
    if (len > 0 && (size_t)len < sizeof(sql)) {
        snprintf(sql + len, sizeof(sql) - len, " ORDER BY id DESC");
    }

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    int param_index = 1;
    if (use_tag_filter) {
        sqlite3_bind_int(stmt, param_index++, tag_filter);
    }
    if (use_status_filter) {
        sqlite3_bind_int(stmt, param_index++, status_filter);
    }

    int capacity = 10;
    *issues = (Issue *)malloc(capacity * sizeof(Issue));
    if (*issues == NULL) {
        sqlite3_finalize(stmt);
        return 0;
    }

    *count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (*count >= capacity) {
            capacity *= 2;
            Issue *new_issues = (Issue *)realloc(*issues, capacity * sizeof(Issue));
            if (new_issues == NULL) {
                free(*issues);
                *issues = NULL;
                sqlite3_finalize(stmt);
                return 0;
            }
            *issues = new_issues;
        }

        Issue *current = &(*issues)[*count];
        current->id = sqlite3_column_int(stmt, 0);
        current->tag = (IssueTag)sqlite3_column_int(stmt, 1);
        current->status = (IssueStatus)sqlite3_column_int(stmt, 2);

        const char *name = (const char *)sqlite3_column_text(stmt, 3);
        const char *detail = (const char *)sqlite3_column_text(stmt, 4);
        const char *user_id = (const char *)sqlite3_column_text(stmt, 5);

        strncpy(current->name, name ? name : "", sizeof(current->name) - 1);
        strncpy(current->detail, detail ? detail : "", sizeof(current->detail) - 1);
        strncpy(current->user_id, user_id ? user_id : "", sizeof(current->user_id) - 1);

        current->name[sizeof(current->name) - 1] = '\0';
        current->detail[sizeof(current->detail) - 1] = '\0';
        current->user_id[sizeof(current->user_id) - 1] = '\0';

        (*count)++;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int update_issue_status(int id, IssueStatus new_status) {
    sqlite3 *db = get_database();
    if (db == NULL) {
        return 0;
    }

    const char *sql = "UPDATE zako SET status = ? WHERE id = ?";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, new_status);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    return sqlite3_changes(db) > 0 ? 1 : 0;
}

int delete_issue(int id) {
    sqlite3 *db = get_database();
    if (db == NULL) {
        return 0;
    }

    const char *sql = "UPDATE zako SET status = ?, name = ?, detail = ? WHERE id = ?";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, STATUS_DELETED);
    sqlite3_bind_text(stmt, 2, "Deleted Issue", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, "Deleted by admin", -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    return sqlite3_changes(db) > 0 ? 1 : 0;
}
