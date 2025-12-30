FROM node:18-alpine AS builder

RUN apk add --no-cache build-base python3 sqlite-dev

WORKDIR /app

COPY package*.json ./
RUN npm ci

COPY tsconfig.json binding.gyp ./
COPY src/ ./src/
COPY native/ ./native/

RUN npm run build

FROM node:18-alpine AS deps

WORKDIR /app
COPY package*.json ./
RUN npm ci --omit=dev

FROM node:18-alpine

RUN apk add --no-cache sqlite-libs

RUN addgroup -g 1001 -S nodejs && \
    adduser -S nodejs -u 1001

WORKDIR /app

COPY --from=builder /app/dist ./dist
COPY --from=builder /app/build ./build
COPY --from=deps /app/node_modules ./node_modules
COPY package.json ./

RUN mkdir -p /app/data && chown -R nodejs:nodejs /app

USER nodejs

CMD ["node", "dist/index.js"]
