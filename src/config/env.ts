import * as dotenv from 'dotenv';

dotenv.config();

export interface Config {
  discordToken: string;
  sqliteFile: string;
  adminIds: string[];
  embedPageSize: number;
}

function validateEnv(): Config {
  const discordToken = process.env.DISCORD_TOKEN;
  if (!discordToken) {
    throw new Error('DISCORD_TOKEN environment variable is required');
  }

  const sqliteFile = process.env.SQLITE_FILE || './data/issues.db';

  const adminIdsStr = process.env.ADMIN_IDS || '';
  const adminIds = adminIdsStr.split(',').map(id => id.trim()).filter(id => id.length > 0);

  const embedPageSize = parseInt(process.env.EMBED_PAGE_SIZE || '5', 10);
  if (isNaN(embedPageSize) || embedPageSize < 1) {
    throw new Error('EMBED_PAGE_SIZE must be a positive number');
  }

  return {
    discordToken,
    sqliteFile,
    adminIds,
    embedPageSize
  };
}

export const config = validateEnv();
