#ifndef ISSUE_OPS_H
#define ISSUE_OPS_H

#include "../include/types.h"

int create_issue(const char *name, const char *detail, IssueTag tag, const char *user_id);
int get_issue_by_id(int id, Issue *issue);
int list_issues(int tag_filter, int status_filter, int use_tag_filter, int use_status_filter, Issue **issues, int *count);
int update_issue_status(int id, IssueStatus new_status);
int delete_issue(int id);

#endif // ISSUE_OPS_H
