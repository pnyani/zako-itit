import { Issue } from '../types/issue';

export interface PaginatedResult<T> {
  items: T[];
  totalPages: number;
  currentPage: number;
  hasNext: boolean;
  hasPrevious: boolean;
  totalCount: number;
}

export function paginateIssues(
  issues: Issue[],
  page: number,
  pageSize: number
): PaginatedResult<Issue> {
  const totalCount = issues.length;
  const totalPages = Math.ceil(totalCount / pageSize);
  const currentPage = Math.max(1, Math.min(page, totalPages || 1));

  const start = (currentPage - 1) * pageSize;
  const end = start + pageSize;

  return {
    items: issues.slice(start, end),
    totalPages,
    currentPage,
    hasNext: end < totalCount,
    hasPrevious: currentPage > 1,
    totalCount
  };
}
