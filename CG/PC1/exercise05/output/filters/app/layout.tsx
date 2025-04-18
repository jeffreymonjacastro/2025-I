import type { Metadata } from "next";
import "./globals.css";

export const metadata: Metadata = {
  title: "Filters",
  description: "Jeffrey Monja Castro",
  icons: {
    icon: "./icon.png",
  }
};


export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en">
      <body className="dark">
        {children}
      </body>
    </html>
  );
}
