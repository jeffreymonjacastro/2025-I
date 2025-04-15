import type { Metadata } from "next";
import "./globals.css";

export const metadata: Metadata = {
  title: "Filters",
  description: "Jeffrey Monja Castro",
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en">
      <body>
        {children}
      </body>
    </html>
  );
}
