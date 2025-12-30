import { config } from '../config/env';

export function isAdmin(userId: string): boolean {
  return config.adminIds.includes(userId);
}
